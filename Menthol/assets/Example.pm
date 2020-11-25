Scene: Untitled
Entities:
  - Entity: 12837192831273
    TagComponent:
      Tag: Green Square
    TransformComponent:
      Translation: [1.5, 0, -29]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
  - Entity: 12837192831273
    TagComponent:
      Tag: Red Square
    TransformComponent:
      Translation: [0, 1, 6.19999981]
      Rotation: [1.20253181, 0, 0.450294942]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0, 0, 1]
  - Entity: 12837192831273
    TagComponent:
      Tag: Camera A
    TransformComponent:
      Translation: [0, 0, 9.5]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
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
  - Entity: 12837192831273
    TagComponent:
      Tag: Camera B
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: false
      FixedAspectRatio: false