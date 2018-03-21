using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using Ivi.Driver;
using Ivi.Scope;
using RohdeSchwarz.RsScope;


namespace DelayMeasurement
{
    class Measurement : IDisposable
    {
        private SerialPort COMPort;
        private List<Task> tasks;
        private List<Thread> threads;
        private RsScope driver;
        private string workDirectory;

        private CancellationTokenSource cancellationRequester;

        public Measurement(string _COMPort, string ip, string directory, string loadSaveFile, int configMode)
        {
            tasks = new List<Task>();
            threads = new List<Thread>();
            if (!Directory.Exists(directory))
                throw new Exception("directory");
            else
                workDirectory = directory;

            try
            {
                driver = new RsScope("TCPIP::" + ip + "::HISLIP", true, false, "Simulate=False");
            }
            catch (Exception)
            {
                this.Dispose(true);
                throw new Exception("connection");
            }
            switch (configMode)
            {
                case 0: //config from file
                    if (!File.Exists(loadSaveFile))
                    {
                        this.Dispose(true);
                        throw new Exception("configFile");
                    }
                    else
                    {
                        try
                        {
                            driver.DataManagement.WriteFromFileToInstrument(loadSaveFile, @"C:\Temp\transferDFL.temp");
                            driver.DataManagement.RecallInstrumentsSettings(0, @"C:\Temp\transferDFL.temp");
                            driver.DataManagement.Recall(0);
                            driver.DataManagement.DeleteFile(@"C:\Temp\transferDFL.temp");
                        }
                        catch (Exception)
                        {
                            this.Dispose(true);
                            throw new Exception("connection");
                        }
                    }
                    break;
                case 1: //save to file
                    if (!Directory.Exists(Path.GetDirectoryName(loadSaveFile)))
                    {
                        this.Dispose(true);
                        throw new Exception("saveDirectory");
                    }
                    else
                    {
                        try
                        {
                            driver.DataManagement.Save(0);
                            driver.DataManagement.SaveInstrumentSettings(0, @"C:\Temp\transferDFL.temp");
                            driver.DataManagement.ReadToFileFromInstrument(@"C:\Temp\transferDFL.temp", loadSaveFile);
                            driver.DataManagement.DeleteFile(@"C:\Temp\transferDFL.temp");
                        }
                        catch (Exception)
                        {
                            this.Dispose(true);
                            throw new Exception("connection");
                        }
                    }
                    break;
            }
            try
            {
                COMPort = new SerialPort(_COMPort, 115200, Parity.None, 8, StopBits.One);
            }
            catch (Exception)
            {
                this.Dispose(true);
                throw new Exception("COMPort");
            }

            cancellationRequester = new CancellationTokenSource();
            tasks.Add(Task.Run(() => Start()));
        }
        ~Measurement()
        {
            try
            {
                Dispose(false);
            }
            catch (Exception)
            { }
        }
        public void Dispose()
        {
            try
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            catch (Exception)
            { }
        }
        public void Dispose(bool freeManagedResources)
        {
            if (freeManagedResources)
            {
                try
                {
                    foreach (Task task in tasks)
                    {
                        task.Wait();
                    }
                    foreach (Thread thread in threads)
                    {
                        thread.Join();
                    }
                    driver.Close();
                    COMPort.Close();
                }
                catch (Exception)
                { }
            }
        }
        public void terminationHandler(object sender, FormClosingEventArgs e)
        {
            this.Close();
        }
        private void Start()
        {
            //!!!!!!!!!!!!!!!
        }
        public void Close()
        {
            Dispose(true);
        }
    }
}
