using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CL
{
    public partial class UC : UserControl
    {
        public UC()
        {
            InitializeComponent();
        }

        [Browsable(true)]
        public string UCName
        {
            get
            {
                return ucName;
            }
            set
            {
                ucName = value;

                this.OnMouseClick(null);
            }
        }

        private void UC_Load(object sender, EventArgs e)
        {

        }

        private string ucName;
    }
}
