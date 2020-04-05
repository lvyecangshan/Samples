using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathLib1;

namespace ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            MyMathWrapper1 m = new MyMathWrapper1();

            Console.WriteLine(m.Add(1, 2));

            Console.Read();
        }
    }
}
