using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DelayMeasurement
{
    class Measurement : IDisposable
    {
        Measurement()
        {

        }
        ~Measurement()
        {
            try
            {
                Dispose(false);
            }
            finally { }
        }
        public void Dispose()
        {
            try
            {
                Dispose(true);
                GC.SuppressFinalize(this);
            }
            finally { }
        }
        protected void Dispose(bool freeManagedResources)
        {

        }
        public void terminationHandler(object sender, FormClosingEventArgs e)
        {
            Dispose(true);
        }
        public void Start()
        {

        }

        public void saveConfigurationToFile(string path)
        {

        }
        public void applyConfigurationFile(string path)
        {

        }
    }
}
