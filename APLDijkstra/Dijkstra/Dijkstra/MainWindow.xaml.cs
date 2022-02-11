using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
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

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "linearVersion")]
        public static extern void CpplinearVersion(StringBuilder start);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "parallelVersionVer1")]
        public static extern void CppparallelVersionVer1(StringBuilder start, int nth);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "parallelVersionVer2")]
        public static extern void CppparallelVersionVer2(StringBuilder start, int nth);

        [DllImport("../../../../x64/Debug/DijkstraCpp.dll", EntryPoint = "parallelVersionVer3")]
        public static extern void CppparallelVersionVer3(StringBuilder start, int nth);



        public MainWindow()
        {
            InitializeComponent();

            StringBuilder cities1 = new StringBuilder(4096);
            CimportFromFile(new StringBuilder("../../../test.txt"), new StringBuilder("Krakow"), cities1);

            AsmBlock.Text = cities1.ToString();


            CppimportFromFile(new StringBuilder("../../../test.txt"));

            Cppalphabet();

            CpplinearVersion(new StringBuilder("Krakow"));

            StringBuilder cities2 = new StringBuilder(4096);
            CppreadListOfCities(cities2);

            CBlock.Text = cities2.ToString();
        }
    }
}