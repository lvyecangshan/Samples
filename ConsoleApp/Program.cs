using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using MathLib1;

namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            MyMathWrapper1 m1 = new MyMathWrapper1();

            Console.WriteLine(m1.Add(1, 2));

            Console.WriteLine(Add(1, 2));

            Console.Read();
        }

        [DllImport("MathLib.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Add(int a, int b);
    }
}
