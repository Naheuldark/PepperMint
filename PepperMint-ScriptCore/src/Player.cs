using System;

namespace Sandbox {

public class Player : PepperMint.Entity {
    private PepperMint.RigidBody2DComponent _rigidbody;

    void OnCreate() { _rigidbody = Get<PepperMint.RigidBody2DComponent>(); }

    void OnUpdate(float timestep) {
        float              speed    = 0.1f;
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
