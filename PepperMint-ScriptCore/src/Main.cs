using System;
using System.Runtime.CompilerServices;

namespace PepperMint {

public struct Vector3 {
    public float x, y, z;

    public Vector3(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
}

public static class InternalCalls {
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void NativeLog(string iMessage, int iValue);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void NativeLog_Vector(ref Vector3 iVector, out Vector3 oResult);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float NativeLog_VectorDot(ref Vector3 iVector);
}
public class Entity {
    public Entity() {
        Console.WriteLine("Main Constructor");

        Log("Script Engine", 8058);

        Vector3 pos    = new Vector3(5, 2.5f, 1);
        Vector3 result = Log(pos);
        Console.WriteLine($"{result.x}, {result.y}, {result.z}");
        Console.WriteLine("{0}", InternalCalls.NativeLog_VectorDot(ref pos));
    }

    // Public functions
    public void PrintHello() { Console.WriteLine("Hello World from C#!"); }
    public void PrintInt(int iValue) { Console.WriteLine($"C# says: {iValue}"); }
    public void PrintInts(int iValue1, int iValue2) { Console.WriteLine($"C# says: {iValue1} and {iValue2}"); }
    public void PrintMessage(string iMessage) { Console.WriteLine($"C# says: {iMessage}"); }

    // Private functions
    private void    Log(string iMessage, int iValue) { InternalCalls.NativeLog(iMessage, iValue); }
    private Vector3 Log(Vector3 iParameter) {
        InternalCalls.NativeLog_Vector(ref iParameter, out Vector3 result);
        return result;
    }
}
}
