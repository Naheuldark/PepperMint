using System;

namespace PepperMint {
public class Main {
    public Main() { Console.WriteLine("Main Constructor"); }

    public void PrintHello() { Console.WriteLine("Hello World from C#!"); }
    public void PrintInt(int value) { Console.WriteLine($"C# says: {value}"); }
    public void PrintInts(int value1, int value2) { Console.WriteLine($"C# says: {value1} and {value2}"); }
    public void PrintMessage(string message) { Console.WriteLine($"C# says: {message}"); }
}
}
