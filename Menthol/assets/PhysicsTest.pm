Scene: Editor
Entities:
  - Entity: 12341723088197825861
    TagComponent:
      Tag: Floor
    TransformComponent:
      Translation: [-1.99718428, -0.632386684, -3.52312941e-06]
      Rotation: [0, 0, 0]
      Scale: [20, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0.421242118, 0, 1]
    RigidBody2DComponent:
      BodyType: 0
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0.200000003
      RestitutionThreshold: 0.5
  - Entity: 3057539796088689264
    TagComponent:
      Tag: Floor
    TransformComponent:
      Translation: [9.68614769, 6.33339262, -3.54984331e-06]
      Rotation: [0, 0, 0.538798332]
      Scale: [19.9999905, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0.421242118, 0, 1]
    RigidBody2DComponent:
      BodyType: 0
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0.200000003
      RestitutionThreshold: 0.5
  - Entity: 12837192831210
    TagComponent:
      Tag: Camera
    TransformComponent:
      Translation: [2.69274592, 6.10296726, 33.8046646]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0.0795452595, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 5902434999490041011
    TagComponent:
      Tag: Circle
    TransformComponent:
      Translation: [-7.81348991, 5.62852573, 0]
      Rotation: [0, 0, 0]
      Scale: [2, 2, 2]
    CircleRendererComponent:
      Color: [1, 0, 0.966696739, 1]
      Thickness: 1
      Fade: 0.00499999989
  - Entity: 12837192831212
    TagComponent:
      Tag: Blob
    TransformComponent:
      Translation: [4.32460594, 13.7430401, -7.35715275e-06]
      Rotation: [0, 0, 1.20966089]
      Scale: [0.999999762, 0.999999762, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
    RigidBody2DComponent:
      BodyType: 1
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5