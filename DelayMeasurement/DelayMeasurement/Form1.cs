using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.WindowsAPICodePack.Dialogs;
using System.IO.Ports;
using System.Management;
using System.Text.RegularExpressions;

namespace DelayMeasurement
{
    public partial class mainWindow : Form
    {
        private Measurement measurement = null;
        private CommonOpenFileDialog folderSelecter;
        private string[] COMPorts;
        /*Task COMPortRefresher;*/
        CancellationTokenSource Cancel_COMPortRefresher;
        public mainWindow()
        {
            InitializeComponent();

            folderSelecter = new CommonOpenFileDialog();
            folderSelecter.EnsurePathExists = true;
            folderSelecter.EnsureFileExists = false;
            folderSelecter.IsFolderPicker = true;
            Cancel_COMPortRefresher = new CancellationTokenSource();
            COMPortRefresher(Cancel_COMPortRefresher.Token);

            textBox_ip.Text = "192.168.1.7";
            textBox_ip.KeyPress += inputCheck_IP;

            comboBox_COM.Sorted = false;
            if (comboBox_COM.Items.Count != 0)
                comboBox_COM.SelectedIndex = 0;
            comboBox_COM.Click += (object sender, EventArgs e) => refreshPortNames();

            radioButton_configure.CheckedChanged += service_radioButton_configure;
            label_loadSaveFile.Text = "Configuration file:";
            label_loadSaveFile.Visible = true;
            textBox_loadSaveFile.Text = "";
            textBox_loadSaveFile.Visible = true;
            button_Browse_loadSaveFile.Visible = true;

            radioButton_loadAndSave.CheckedChanged += service_radioButton_loadAndSave;

            radioButton_leaveAlone.CheckedChanged += service_radioButton_leaveAlone;

            comboBox_COM.Select();
        }

        private void mainWindow_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (measurement != null)
                this.measurement.terminationHandler(sender, e);
        }

        private void button_Browse_Click(object sender, EventArgs e)
        {
            if (folderSelecter.ShowDialog() == CommonFileDialogResult.Ok)
                textBox_directory.Text = folderSelecter.FileName;
        }

        private void refreshPortNames()
        {
            using (ManagementObjectSearcher searcher = new ManagementObjectSearcher("SELECT * FROM WIN32_SerialPort"))
            {
                string selectedCOMPort = (comboBox_COM.SelectedIndex == -1) ? "" : COMPorts[comboBox_COM.SelectedIndex];
                COMPorts = SerialPort.GetPortNames().OrderBy(x => Convert.ToUInt32(x.Substring(3))).ToArray();
                var ports = searcher.Get().Cast<ManagementBaseObject>().ToList();
                comboBox_COM.Items.Clear();
                comboBox_COM.Items.AddRange((from n in COMPorts join p in ports on n equals p["DeviceID"].ToString() orderby Convert.ToUInt32(n.Substring(3)) select p["Caption"].ToString()).ToArray());
                if (selectedCOMPort != "")
                    comboBox_COM.SelectedIndex = Array.FindIndex(COMPorts, x => (x == selectedCOMPort));
                else
                    comboBox_COM.SelectedIndex = -1;
            }
        }
        private async void COMPortRefresher(CancellationToken cancellationRequest)
        {
            while (!cancellationRequest.IsCancellationRequested)
            {
                refreshPortNames();
                await Task.Delay(TimeSpan.FromSeconds(5));
            }
        }
        private string prevText;
        private int prevPosition;
        private int prevSelectionlenght;
        private bool undoable = false;
        private void inputCheck_IP(object sender, EventArgs args)
        {
            char[] chars = new char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\b', '.' };
            KeyPressEventArgs keyevent = (KeyPressEventArgs)args;

            keyevent.Handled = false;

            if (keyevent.KeyChar == '\u0001' || keyevent.KeyChar == '\u0003')   //Ctrl+A and Ctrl+C
                return;

            if (keyevent.KeyChar == '\u001a')   //Ctrl+Z
            {
                if (undoable)
                {
                    textBox_ip.Text = prevText;
                    textBox_ip.SelectionStart = prevPosition;
                    textBox_ip.SelectionLength = prevSelectionlenght;
                }
                undoable = false;
                return;
            }
            else
            {
                prevText = textBox_ip.Text;
                prevPosition = textBox_ip.SelectionStart;
                prevSelectionlenght = textBox_ip.SelectionLength;
                undoable = true;
            }

            string newText = (keyevent.KeyChar == '\b') ? ((textBox_ip.SelectionLength == 0 && textBox_ip.SelectionStart != 0) ? textBox_ip.Text.Remove(textBox_ip.SelectionStart - 1, 1) : textBox_ip.Text.Remove(textBox_ip.SelectionStart, textBox_ip.SelectionLength)) : textBox_ip.Text.Remove(textBox_ip.SelectionStart, textBox_ip.SelectionLength).Insert(textBox_ip.SelectionStart, (keyevent.KeyChar == '\u0016' ? Clipboard.GetText() : keyevent.KeyChar.ToString()));

            keyevent.Handled = true;

            if (keyevent.KeyChar == '\u001b')    //Esc
            {
                textBox_ip.Text = "";
                return;
            }

            if (newText.Count(x => x == '.') > 3 || (newText.Length > 0 && newText.ElementAt(0) == '.'))
                return;

            Regex invalidChar = new Regex(@"[^0-9.]");
            Regex moreThan3Dig = new Regex(@"[0-9]{4,}");
            Regex AdjacentDots = new Regex(@"\.{2,}");
            Regex numbs = new Regex(@"(?<=(^|\.))[0-9]+?(?=(\.|$))");

            if (invalidChar.IsMatch(newText) || moreThan3Dig.IsMatch(newText) || AdjacentDots.IsMatch(newText) || numbs.Matches(newText).Cast<Match>().Select(x => Convert.ToInt32(x.Value)).Count(x => x > 255) != 0)
                return;


            keyevent.Handled = false;
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            try
            {
                measurement = new Measurement();
            }
            catch(Exception)
            {

            }

            
        }

        private void button_Browse_loadSaveFile_Click(object sender, EventArgs e)
        {
            if (radioButton_loadAndSave.Checked)
            {
                saveFileDialog.Filter = "Rohde & Schwarz Configuration | *.dfl";
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                    textBox_loadSaveFile.Text = saveFileDialog.FileName;
            }
            else if (radioButton_configure.Checked)
            {
                openFileDialog.Filter = "Rohde & Schwarz Configuration File | *.dfl";
                if (openFileDialog.ShowDialog() == DialogResult.OK)
                    textBox_loadSaveFile.Text = openFileDialog.FileName;
            }
        }
        private void service_radioButton_configure(object sender, EventArgs args)
        {
            if (radioButton_configure.Checked)
            {
                label_loadSaveFile.Text = "Configuration file:";
                label_loadSaveFile.Visible = true;
                textBox_loadSaveFile.Text = "";
                textBox_loadSaveFile.Visible = true;
                button_Browse_loadSaveFile.Visible = true;
            }
        }
        private void service_radioButton_loadAndSave(object sender, EventArgs args)
        {
            if (radioButton_loadAndSave.Checked)
            {
                label_loadSaveFile.Text = "Configuration file destination:";
                label_loadSaveFile.Visible = true;
                textBox_loadSaveFile.Text = "";
                textBox_loadSaveFile.Visible = true;
                button_Browse_loadSaveFile.Visible = true;
            }
        }
        private void service_radioButton_leaveAlone(object sender, EventArgs args)
        {
            if (radioButton_leaveAlone.Checked)
            {
                label_loadSaveFile.Visible = false;
                textBox_loadSaveFile.Visible = false;
                button_Browse_loadSaveFile.Visible = false;
            }
        }
    }
}
