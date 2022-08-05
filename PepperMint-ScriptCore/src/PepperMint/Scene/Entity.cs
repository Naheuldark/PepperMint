using System;

namespace PepperMint {
public class Entity {
    protected Entity() { UUID = 0; }
    internal Entity(ulong uuid) { UUID = uuid; }

    // Methods
    public bool Has<T>()
        where   T : Component, new() {
          Type componentType = typeof(T);
          return InternalCalls.Entity_HasComponent(UUID, componentType);
    }

    public T  Get<T>()
        where T : Component, new() {
        if (!Has<T>()) {
            return null;
        }

        T component = new T() { Entity = this };
        return component;
    }

    // Attributes
    public readonly ulong UUID;
    public Vector3        Translation {
        get {
            InternalCalls.TransformComponent_GetTranslation(UUID, out Vector3 result);
            return result;
        }
        set { InternalCalls.TransformComponent_SetTranslation(UUID, ref value); }
    }
}
}
