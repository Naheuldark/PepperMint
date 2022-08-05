namespace PepperMint {
public struct Vector2 {
    public Vector2(float scalar) {
        X = scalar;
        Y = scalar;
    }
    public Vector2(float x, float y) {
        X = x;
        Y = y;
    }

    public static Vector2 Zero => new Vector2(0.0f);

    // Operators
    public static Vector2 operator +(Vector2 lhs, Vector2 rhs) { return new Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y); }
    public static Vector2 operator*(Vector2 vec, float scalar) { return new Vector2(vec.X * scalar, vec.Y * scalar); }

    // Attributes
    public float X, Y;
}
}
