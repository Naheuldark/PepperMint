using System;

namespace Sandbox {

public class Player : PepperMint.Entity {

    public float Speed;
    public float Time = 0.0f;

    private PepperMint.RigidBody2DComponent _rigidbody;

    void OnCreate() {
        Console.WriteLine($"Player.OnCreate - {UUID}");

        _rigidbody = Get<PepperMint.RigidBody2DComponent>();
    }

    void OnUpdate(float timestep) {
        Time += timestep;

        float              speed    = Speed;
        PepperMint.Vector3 velocity = PepperMint.Vector3.Zero;

        if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.W)) {
            velocity.Y = 1.0f;
        } else if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.S)) {
            velocity.Y = -1.0f;
        }

        if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.A)) {
            velocity.X = -1.0f;
        } else if (PepperMint.Input.IsKeyDown(PepperMint.KeyCode.D)) {
            velocity.X = 1.0f;
        }

        velocity *= speed;
        _rigidbody.ApplyLinearImpulse(velocity.XY, true);
    }
}
}
