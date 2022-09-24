namespace Sandbox {

public class Camera : PepperMint.Entity {

    public PepperMint.Entity OtherEntity;

    void OnUpdate(float timestep) {
        float              speed    = 1.0f;
        PepperMint.Vector3 velocity = PepperMint.Vector3.Zero;

        if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.UP))
            velocity.Y = 1.0f;
        else if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.DOWN))
            velocity.Y = -1.0f;

        if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.LEFT))
            velocity.X = -1.0f;
        else if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.RIGHT))
            velocity.X = 1.0f;

        velocity *= speed;

        PepperMint.Vector3 translation = Translation;
        translation += velocity * timestep;
        Translation = translation;
    }
}
}
