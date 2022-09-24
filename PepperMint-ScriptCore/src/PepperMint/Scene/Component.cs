﻿namespace PepperMint {
public abstract class Component {
    public Entity Entity { get; internal set; }
}

public class TransformComponent : Component {
    public Vector3 Translation {
        get {
            InternalCalls.TransformComponent_GetTranslation(Entity.UUID, out Vector3 translation);
            return translation;
        }
        set { InternalCalls.TransformComponent_SetTranslation(Entity.UUID, ref value); }
    }
}

public class RigidBody2DComponent : Component {
    public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake) {
        InternalCalls.RigidBody2DComponent_ApplyLinearImpulse(Entity.UUID, ref impulse, ref worldPosition, wake);
    }
    public void ApplyLinearImpulse(Vector2 impulse, bool wake) {
        InternalCalls.RigidBody2DComponent_ApplyLinearImpulseToCenter(Entity.UUID, ref impulse, wake);
    }
}
}