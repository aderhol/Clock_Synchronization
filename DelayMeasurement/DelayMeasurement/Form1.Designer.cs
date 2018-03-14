namespace DelayMeasurement
{
    partial class mainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(mainWindow));
            this.label_IP = new System.Windows.Forms.Label();
            this.textBox_ip = new System.Windows.Forms.TextBox();
            this.label_COM = new System.Windows.Forms.Label();
            this.comboBox_COM = new System.Windows.Forms.ComboBox();
            this.label_directory = new System.Windows.Forms.Label();
            this.textBox_directory = new System.Windows.Forms.TextBox();
            this.button_Browse = new System.Windows.Forms.Button();
            this.button_start = new System.Windows.Forms.Button();
            this.radioButton_loadAndSave = new System.Windows.Forms.RadioButton();
            this.radioButton_configure = new System.Windows.Forms.RadioButton();
            this.radioButton_leaveAlone = new System.Windows.Forms.RadioButton();
            this.button_Browse_loadSaveFile = new System.Windows.Forms.Button();
            this.textBox_loadSaveFile = new System.Windows.Forms.TextBox();
            this.label_loadSaveFile = new System.Windows.Forms.Label();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // label_IP
            // 
            this.label_IP.AutoSize = true;
            this.label_IP.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_IP.Location = new System.Drawing.Point(12, 9);
            this.label_IP.Name = "label_IP";
            this.label_IP.Size = new System.Drawing.Size(193, 20);
            this.label_IP.TabIndex = 0;
            this.label_IP.Text = "Oscilloscope\'s IP address:";
            // 
            // textBox_ip
            // 
            this.textBox_ip.Location = new System.Drawing.Point(211, 11);
            this.textBox_ip.MaxLength = 15;
            this.textBox_ip.Name = "textBox_ip";
            this.textBox_ip.Size = new System.Drawing.Size(88, 20);
            this.textBox_ip.TabIndex = 0;
            // 
            // label_COM
            // 
            this.label_COM.AutoSize = true;
            this.label_COM.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_COM.Location = new System.Drawing.Point(12, 45);
            this.label_COM.Name = "label_COM";
            this.label_COM.Size = new System.Drawing.Size(148, 20);
            this.label_COM.TabIndex = 2;
            this.label_COM.Text = "Module\'s COM port:";
            // 
            // comboBox_COM
            // 
            this.comboBox_COM.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_COM.FormattingEnabled = true;
            this.comboBox_COM.Items.AddRange(new object[] {
            "a",
            "b",
            "c"});
            this.comboBox_COM.Location = new System.Drawing.Point(211, 47);
            this.comboBox_COM.Name = "comboBox_COM";
            this.comboBox_COM.Size = new System.Drawing.Size(284, 21);
            this.comboBox_COM.TabIndex = 1;
            // 
            // label_directory
            // 
            this.label_directory.AutoSize = true;
            this.label_directory.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_directory.Location = new System.Drawing.Point(12, 90);
            this.label_directory.Name = "label_directory";
            this.label_directory.Size = new System.Drawing.Size(114, 20);
            this.label_directory.TabIndex = 4;
            this.label_directory.Text = "Work directory:";
            // 
            // textBox_directory
            // 
            this.textBox_directory.Location = new System.Drawing.Point(211, 92);
            this.textBox_directory.Name = "textBox_directory";
            this.textBox_directory.Size = new System.Drawing.Size(203, 20);
            this.textBox_directory.TabIndex = 3;
            // 
            // button_Browse
            // 
            this.button_Browse.Location = new System.Drawing.Point(420, 90);
            this.button_Browse.Name = "button_Browse";
            this.button_Browse.Size = new System.Drawing.Size(75, 23);
            this.button_Browse.TabIndex = 2;
            this.button_Browse.Text = "Browse";
            this.button_Browse.UseVisualStyleBackColor = true;
            this.button_Browse.Click += new System.EventHandler(this.button_Browse_Click);
            // 
            // button_start
            // 
            this.button_start.Font = new System.Drawing.Font("Microsoft Sans Serif", 36F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_start.Location = new System.Drawing.Point(12, 271);
            this.button_start.Name = "button_start";
            this.button_start.Size = new System.Drawing.Size(483, 75);
            this.button_start.TabIndex = 9;
            this.button_start.Text = "Start measurement";
            this.button_start.UseVisualStyleBackColor = true;
            this.button_start.Click += new System.EventHandler(this.button_start_Click);
            // 
            // radioButton_loadAndSave
            // 
            this.radioButton_loadAndSave.AutoSize = true;
            this.radioButton_loadAndSave.Location = new System.Drawing.Point(16, 161);
            this.radioButton_loadAndSave.Name = "radioButton_loadAndSave";
            this.radioButton_loadAndSave.Size = new System.Drawing.Size(241, 17);
            this.radioButton_loadAndSave.TabIndex = 5;
            this.radioButton_loadAndSave.Text = "load and save the instruments current settings";
            this.radioButton_loadAndSave.UseVisualStyleBackColor = true;
            // 
            // radioButton_configure
            // 
            this.radioButton_configure.AutoSize = true;
            this.radioButton_configure.Checked = true;
            this.radioButton_configure.Location = new System.Drawing.Point(16, 138);
            this.radioButton_configure.Name = "radioButton_configure";
            this.radioButton_configure.Size = new System.Drawing.Size(186, 17);
            this.radioButton_configure.TabIndex = 4;
            this.radioButton_configure.TabStop = true;
            this.radioButton_configure.Text = "configure the instrument from a file";
            this.radioButton_configure.UseVisualStyleBackColor = true;
            // 
            // radioButton_leaveAlone
            // 
            this.radioButton_leaveAlone.AutoSize = true;
            this.radioButton_leaveAlone.Location = new System.Drawing.Point(16, 184);
            this.radioButton_leaveAlone.Name = "radioButton_leaveAlone";
            this.radioButton_leaveAlone.Size = new System.Drawing.Size(186, 17);
            this.radioButton_leaveAlone.TabIndex = 6;
            this.radioButton_leaveAlone.Text = "keep the instruments settings as is";
            this.radioButton_leaveAlone.UseVisualStyleBackColor = true;
            // 
            // button_Browse_loadSaveFile
            // 
            this.button_Browse_loadSaveFile.Location = new System.Drawing.Point(420, 216);
            this.button_Browse_loadSaveFile.Name = "button_Browse_loadSaveFile";
            this.button_Browse_loadSaveFile.Size = new System.Drawing.Size(75, 23);
            this.button_Browse_loadSaveFile.TabIndex = 7;
            this.button_Browse_loadSaveFile.Text = "Browse";
            this.button_Browse_loadSaveFile.UseVisualStyleBackColor = true;
            this.button_Browse_loadSaveFile.Visible = false;
            this.button_Browse_loadSaveFile.Click += new System.EventHandler(this.button_Browse_loadSaveFile_Click);
            // 
            // textBox_loadSaveFile
            // 
            this.textBox_loadSaveFile.Location = new System.Drawing.Point(232, 218);
            this.textBox_loadSaveFile.Name = "textBox_loadSaveFile";
            this.textBox_loadSaveFile.Size = new System.Drawing.Size(182, 20);
            this.textBox_loadSaveFile.TabIndex = 8;
            this.textBox_loadSaveFile.Visible = false;
            // 
            // label_loadSaveFile
            // 
            this.label_loadSaveFile.AutoSize = true;
            this.label_loadSaveFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_loadSaveFile.Location = new System.Drawing.Point(12, 216);
            this.label_loadSaveFile.Name = "label_loadSaveFile";
            this.label_loadSaveFile.Size = new System.Drawing.Size(214, 20);
            this.label_loadSaveFile.TabIndex = 11;
            this.label_loadSaveFile.Text = "Configuration file destination:";
            this.label_loadSaveFile.Visible = false;
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            // 
            // mainWindow
            // 
            this.AcceptButton = this.button_start;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 358);
            this.Controls.Add(this.button_Browse_loadSaveFile);
            this.Controls.Add(this.textBox_loadSaveFile);
            this.Controls.Add(this.label_loadSaveFile);
            this.Controls.Add(this.radioButton_leaveAlone);
            this.Controls.Add(this.radioButton_configure);
            this.Controls.Add(this.radioButton_loadAndSave);
            this.Controls.Add(this.button_start);
            this.Controls.Add(this.button_Browse);
            this.Controls.Add(this.textBox_directory);
            this.Controls.Add(this.label_directory);
            this.Controls.Add(this.comboBox_COM);
            this.Controls.Add(this.label_COM);
            this.Controls.Add(this.textBox_ip);
            this.Controls.Add(this.label_IP);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "mainWindow";
            this.Text = "PPS skew measurement";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.mainWindow_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_IP;
        private System.Windows.Forms.TextBox textBox_ip;
        private System.Windows.Forms.Label label_COM;
        private System.Windows.Forms.ComboBox comboBox_COM;
        private System.Windows.Forms.Label label_directory;
        private System.Windows.Forms.TextBox textBox_directory;
        private System.Windows.Forms.Button button_Browse;
        private System.Windows.Forms.Button button_start;
        private System.Windows.Forms.RadioButton radioButton_loadAndSave;
        private System.Windows.Forms.RadioButton radioButton_configure;
        private System.Windows.Forms.RadioButton radioButton_leaveAlone;
        private System.Windows.Forms.Button button_Browse_loadSaveFile;
        private System.Windows.Forms.TextBox textBox_loadSaveFile;
        private System.Windows.Forms.Label label_loadSaveFile;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
    }
}

