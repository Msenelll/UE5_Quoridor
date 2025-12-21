graph TD
    %% Base Class
    UObject["UObject<br/>(Garbage Collection<br/>Reflection System)"]
    
    %% Main Branches
    AActor["AActor<br/>(Replication, Spawnable<br/>Tick, Components)"]
    UActorComponent["UActorComponent<br/>(Logic Only<br/>Reusable Behavior)"]
    
    %% ACTOR BRANCHES
    AInfo["AInfo<br/>(Manager Classes)"]
    AGameModeBase["AGameModeBase<br/>(Game Rules<br/>Player Spawning)"]
    AGameStateBase["AGameStateBase<br/>(Game State<br/>Score, Time)"]
    
    AController["AController<br/>(Possess Pawn<br/>Input Handling)"]
    APlayerController["APlayerController<br/>(Human Input<br/>HUD, Camera)"]
    AAIController["AAIController<br/>(AI, Behavior Trees<br/>Navigation)"]
    
    APawn["APawn<br/>(Physical Representation<br/>Movement Input)"]
    ACharacter["ACharacter<br/>(Humanoid Movement<br/>Skeletal Mesh<br/>Jump/Walk/Swim)"]
    
    %% COMPONENT BRANCHES
    USceneComponent["USceneComponent<br/>(Transform<br/>Attachment Hierarchy)"]
    UMovementComponent["UMovementComponent<br/>(Movement Logic)"]
    UCharacterMovementComponent["UCharacterMovementComponent<br/>(Walking Physics<br/>Network Prediction)"]
    
    UPrimitiveComponent["UPrimitiveComponent<br/>(Rendering<br/>Collision, Physics)"]
    UStaticMeshComponent["UStaticMeshComponent<br/>(3D Model Static)"]
    USkeletalMeshComponent["USkeletalMeshComponent<br/>(Animated Model)"]
    UCapsuleComponent["UCapsuleComponent<br/>(Simple Collision)"]
    UCameraComponent["UCameraComponent<br/>(Player View)"]
    
    %% Inheritance Links
    UObject --> AActor
    UObject --> UActorComponent
    
    AActor --> AInfo
    AInfo --> AGameModeBase
    AInfo --> AGameStateBase
    
    AActor --> AController
    AController --> APlayerController
    AController --> AAIController
    
    AActor --> APawn
    APawn --> ACharacter
    
    UActorComponent --> USceneComponent
    UActorComponent --> UMovementComponent
    UMovementComponent --> UCharacterMovementComponent
    
    USceneComponent --> UPrimitiveComponent
    USceneComponent --> UCameraComponent
    UPrimitiveComponent --> UStaticMeshComponent
    UPrimitiveComponent --> USkeletalMeshComponent
    UPrimitiveComponent --> UCapsuleComponent
    
    %% Composition Links
    ACharacter -.Contains.-> UCapsuleComponent
    ACharacter -.Contains.-> USkeletalMeshComponent
    ACharacter -.Contains.-> UCharacterMovementComponent
    
    classDef base fill:#ff6b6b,stroke:#c92a2a,color:#fff
    classDef actor fill:#4ecdc4,stroke:#0e7c86,color:#fff
    classDef component fill:#95e1d3,stroke:#38a169,color:#000
    classDef derived fill:#ffe66d,stroke:#f0a500,color:#000
    
    class UObject base
    class AActor,AController,APawn actor
    class UActorComponent,USceneComponent,UMovementComponent component
    class AGameModeBase,AGameStateBase,APlayerController,AAIController,ACharacter,UPrimitiveComponent,UStaticMeshComponent,USkeletalMeshComponent,UCapsuleComponent,UCameraComponent,UCharacterMovementComponent derived