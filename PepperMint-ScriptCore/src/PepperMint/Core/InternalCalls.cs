using System;
using System.Runtime.CompilerServices;

namespace PepperMint {

public static class InternalCalls {
    // Log
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Log_MessageInt(string message, int value);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void Log_Vector(ref Vector3 vector, out Vector3 result);
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static float Log_VectorDot(ref Vector3 vector);

    // Entity
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Entity_HasComponent(ulong entityUUID, Type componentType);

    // Transform Component
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_GetTranslation(ulong entityUUID, out Vector3 translation);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void TransformComponent_SetTranslation(ulong entityUUID, ref Vector3 translation);

    // Rigid Body 2D Component
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void RigidBody2DComponent_ApplyLinearImpulse(ulong entityUUID, ref Vector2 impulse, ref Vector2 point, bool wake);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static void RigidBody2DComponent_ApplyLinearImpulseToCenter(ulong entityUUID, ref Vector2 impulse, bool wake);

    // Input
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    internal extern static bool Input_IsKeyDown(KeyCode keycode);
}
}
