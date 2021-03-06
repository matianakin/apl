using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
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
        public static extern void CimportFromFile(StringBuilder filename, StringBuilder start, StringBuilder buffer, StringBuilder time);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "importFromFile")]
        public static extern void CPPimportFromFile(StringBuilder filename);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "nodeCheck")]
        public static extern bool CPPnodeCheck(StringBuilder name);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "checkNoOfThreads")]
        public static extern bool CPPcheckNoOfThreads(int threadCount);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "callCPP")]
        public static extern void callCPP(StringBuilder filename, StringBuilder start, int nth, StringBuilder buffer, StringBuilder time);

        bool dirGiven = false;

        public MainWindow()
        {
            InitializeComponent();

        }

        private bool CheckFile(string filename)
        {
            var fileStream = new FileStream(filename, FileMode.Open, FileAccess.Read);
            StreamReader reader = new StreamReader(fileStream, Encoding.UTF8);
            string[] words = reader.ReadToEnd().Split(new Char[] { ',', '\\', '\n', ' ', '.', '-' },
                                 StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < words.Length; i++)
            {
                if (i % 3==2)
                {
                    try
                    {
                        int numVal = Int32.Parse(words[i]);
                        Console.WriteLine(numVal);
                    }
                    catch (FormatException x)
                    {
                        Console.WriteLine(x.Message);
                        return false;
                    }
                }
            }
            return true;
        }

        private void AsmButton_Click(object sender, RoutedEventArgs e)
        {
            if (dirGiven)
            {
                CPPimportFromFile(new StringBuilder(fileBox.Text));


                if (CPPnodeCheck(new StringBuilder(StartNodeBox.Text)))
                {
                    StringBuilder nodes1 = new StringBuilder(1000000);
                    StringBuilder time = new StringBuilder(4096);
                    CimportFromFile(new StringBuilder(fileBox.Text), new StringBuilder(StartNodeBox.Text), nodes1, time);
                    AsmBlock.Text = nodes1.ToString();
                    AsmTime.Text=time.ToString();
                }
                else
                {
                    MessageBox.Show("Please choose a starting node that is included in the chosen file", "Incorrect node name");
                }
            }
            else
            {
                MessageBox.Show("Please choose a .txt file", "No file chosen");
            }
        }

        private void CppButton_Click(object sender, RoutedEventArgs e)
        {
            if (dirGiven)
            {

                CPPimportFromFile(new StringBuilder(fileBox.Text));

                if (CPPnodeCheck(new StringBuilder(StartNodeBox.Text)))
                {
                    int numVal = -1;
                    try
                    {
                        numVal = Int32.Parse(Threadbox.Text);
                        Console.WriteLine(numVal);
                    }
                    catch (FormatException x)
                    {
                        Console.WriteLine(x.Message);
                    }
                    if (numVal == -1)
                    {
                        MessageBox.Show("Please provide desired number of threads", "Error reading no. of Threads");
                    }
                    else if (numVal < 1)
                    {
                        MessageBox.Show("Minimal number of threads is 1", "Incorrect no of Threads");
                    }
                    else if (numVal>64)
                    {
                        MessageBox.Show("Maximum supported number of threads is 64", "Too many threads requested");
                    }
                    else if (CPPcheckNoOfThreads(numVal))
                    {
                        StringBuilder nodes2 = new StringBuilder(1000000);
                        StringBuilder time = new StringBuilder(4096);
                        callCPP(new StringBuilder(fileBox.Text), new StringBuilder(StartNodeBox.Text), numVal, nodes2, time);

                        CppBlock.Text = nodes2.ToString();
                        CppTime.Text = time.ToString();

                    }
                    else
                    {
                        MessageBox.Show("Too many threads requested. Number of threads cannot exceed number of vertices.", "Too many threads requested");
                    }
                }
                else
                {
                    MessageBox.Show("Please choose a starting node that is included in the chosen file", "Incorrect node name");
                }
            }
            else
            {
                MessageBox.Show("Please choose a .txt file", "No file chosen");
            }
        }

        private void BrowseButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Text files (*.txt)|*.txt";
            if (openFileDialog.ShowDialog() == true)
            {
                string directory = System.IO.Path.GetDirectoryName(openFileDialog.FileName) + "\\" + System.IO.Path.GetFileName(openFileDialog.FileName);
                if (CheckFile(directory))
                {
                    fileBox.Text = directory;
                    dirGiven = true;
                }
                else
                {
                    MessageBox.Show("Incorrect .txt file selected \nPlease select another file", "Incorrect file selected");

                }
            }

        }

        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");
            e.Handled = regex.IsMatch(e.Text);

        }
    }
 }