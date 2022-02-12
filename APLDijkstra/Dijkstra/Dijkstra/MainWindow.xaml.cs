using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Dijkstra
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("../../../../x64/Debug/DijkstraC.dll", EntryPoint = "importFromFile")]
        public static extern void CimportFromFile(StringBuilder filename, StringBuilder start, StringBuilder buffer);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "importFromFile")]
        public static extern void CppimportFromFile(StringBuilder filename);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "alphabet")]
        public static extern void Cppalphabet();

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "readListOfCities")]
        public static extern void CppreadListOfCities(StringBuilder buffer);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "parallelVersionVer2")]
        public static extern void CppparallelVersionVer2(StringBuilder start, int nth);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "nodeCheck")]
        public static extern bool nodeCheck(StringBuilder name);

        bool dirGiven=false;

        public MainWindow()
        {
            InitializeComponent();

        }


        private void AsmButton_Click(object sender, RoutedEventArgs e)
        {
            if (dirGiven)
            {
                if (nodeCheck(new StringBuilder(StartCityBox.Text)))
                {
                    StringBuilder cities1 = new StringBuilder(4096);
                    CimportFromFile(new StringBuilder(fileBox.Text), new StringBuilder(StartCityBox.Text), cities1);
                    AsmBlock.Text = cities1.ToString();
                }
                else
                {
                    /*Popup codePopup = new Popup();
                    TextBlock popupText = new TextBlock();
                    popupText.Text = "Popup Text";
                    myPopupText.Text = "Popup";
                    popupText.Background = Brushes.LightBlue;
                    popupText.Foreground = Brushes.Blue;
                    codePopup.Child = popupText;*/
                    myPopupText.Text = "Please choose a starting city that is included in the chosen file";
                    myPopup.IsOpen = true;
                    myPopup.IsOpen = false;
                }
            }
        }

        private void CButton_Click(object sender, RoutedEventArgs e)
        {
            if (dirGiven)
            {

                CppimportFromFile(new StringBuilder(fileBox.Text));

                if (nodeCheck(new StringBuilder(StartCityBox.Text)))
                {

                    Cppalphabet();

                    CppparallelVersionVer2(new StringBuilder(StartCityBox.Text), 3);

                    StringBuilder cities2 = new StringBuilder(4096);
                    CppreadListOfCities(cities2);

                    CBlock.Text = cities2.ToString();
                }
                else
                {
                    /*Popup codePopup = new Popup();
                    TextBlock popupText = new TextBlock();
                    popupText.Text = "Popup Text";
                    popupText.Background = Brushes.LightBlue;
                    popupText.Foreground = Brushes.Blue;
                    codePopup.Child = popupText;*/
                    myPopupText.Text = "Please choose a starting city that is included in the chosen file";
                    myPopup.IsOpen = true;
                    myPopup.IsOpen=false;
                }
            }
        }

        private void BrowseButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == true)
            {
                fileBox.Text = System.IO.Path.GetDirectoryName(openFileDialog.FileName) + "\\" + System.IO.Path.GetFileName(openFileDialog.FileName);
                dirGiven = true;
            }

        }
    }
}