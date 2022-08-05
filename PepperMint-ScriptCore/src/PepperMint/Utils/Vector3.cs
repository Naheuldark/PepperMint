namespace PepperMint {
public struct Vector3 {
    public Vector3(float scalar) {
        X = scalar;
        Y = scalar;
        Z = scalar;
    }
    public Vector3(float x, float y, float z) {
        X = x;
        Y = y;
        Z = z;
    }

    public static Vector3 Zero => new Vector3(0.0f);

    public Vector2 XY {
        get => new Vector2(X, Y);
        set {
            X = value.X;
            Y = value.Y;
        }
    }

    // Operators
    public static Vector3 operator +(Vector3 lhs, Vector3 rhs) { return new Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z); }
    public static Vector3 operator*(Vector3 vec, float scalar) { return new Vector3(vec.X * scalar, vec.Y * scalar, vec.Z * scalar); }

    // Attributes
    public float X, Y, Z;
}
}
