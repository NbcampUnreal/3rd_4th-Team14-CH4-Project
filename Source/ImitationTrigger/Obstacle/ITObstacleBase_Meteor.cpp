#include "Obstacle/ITObstacleBase_Meteor.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

AITObstacleBase_Meteor::AITObstacleBase_Meteor()
{
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->InitSphereRadius(150.f);
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SetRootComponent(Collision);

    MeteorVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MeteorVFX"));
    MeteorVFX->SetupAttachment(RootComponent);
    MeteorVFX->SetAutoActivate(false);

    bReplicates = true;
    SetReplicateMovement(true);

}

void AITObstacleBase_Meteor::BeginPlay()
{
    Super::BeginPlay();

    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        Collision->GetScaledSphereRadius(),
        24,                
        FColor::Red,        
        false,             
        10.f,               
        0,
        2.f                 
    );

    if (HasAuthority())
    {
        if (FallingVFX)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                FallingVFX,
                GetActorLocation() + FVector(0, 0, 800.f),
                FRotator::ZeroRotator,
                FVector(1.0f),
                true,
                true
            );
        }

        GetWorld()->GetTimerManager().SetTimer(
            ImpactTimer,
            this,
            &AITObstacleBase_Meteor::OnImpact,
            ImpactDelay,
            false
        );
    }
}

void AITObstacleBase_Meteor::OnImpact()
{
    if (!HasAuthority())
    {
        return;
    }

    if (ImpactVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ImpactVFX,
            GetActorLocation() + FVector(0, 0, -70.f),
            FRotator::ZeroRotator
        );
    }

    // Collider 범위 내 대상에게 피해
    TArray<AActor*> Overlaps;
    Collision->GetOverlappingActors(Overlaps, ACharacter::StaticClass());

    for (AActor* Target : Overlaps)
    {
        UGameplayStatics::ApplyDamage(
            Target,
            Damage,
            GetInstigatorController(),
            this,
            UDamageType::StaticClass()
        );
    }

    Multicast_PlayImpactVFX();

    Destroy();
}

void AITObstacleBase_Meteor::Multicast_PlayImpactVFX_Implementation()
{
    if (ImpactVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ImpactVFX,
            GetActorLocation() + FVector(0, 0, -70.f),
            FRotator::ZeroRotator
        );
    }
}