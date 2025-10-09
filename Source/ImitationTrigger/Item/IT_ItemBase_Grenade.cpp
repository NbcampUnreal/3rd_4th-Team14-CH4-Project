#include "Item/IT_ItemBase_Grenade.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


AIT_ItemBase_Grenade::AIT_ItemBase_Grenade()
{
    // MeshComp : 물리 적용
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(MeshComp);

    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetEnableGravity(true);
    MeshComp->SetNotifyRigidBodyCollision(true);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
    MeshComp->OnComponentHit.AddDynamic(this, &AIT_ItemBase_Grenade::OnGrenadeHit);

    // 감쇠(공기 저항 및 회전 저항)
    MeshComp->SetLinearDamping(0.1f);   // 공기 저항: 조금 높여서 자연 감속
    MeshComp->SetAngularDamping(0.1f);   // 회전 감쇠: 바닥에서 안정적 굴림
    MeshComp->SetMassOverrideInKg(NAME_None, 2.5f, true); // 무게감 추가

    // 폭발 감지 영역
    ExplosionRadiusComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRadius"));
    ExplosionRadiusComp->SetupAttachment(MeshComp);
    ExplosionRadiusComp->InitSphereRadius(ExplosionRadius);
    ExplosionRadiusComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionRadiusComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    ExplosionRadiusComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AIT_ItemBase_Grenade::BeginPlay()
{
    Super::BeginPlay();

    // 물리 머티리얼 생성 (BeginPlay 시점에서 안전)
    UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
    PhysMat->Friction = 0.4f;       // 마찰력: 약간 낮춰서 잘 굴러가게
    PhysMat->Restitution = 0.8f;    // 반발력: 튕김 정도를 조금 높임
    MeshComp->SetPhysMaterialOverride(PhysMat);

    // 서버만 폭발 타이머 시작
    if (HasAuthority())
    {
        GetWorldTimerManager().SetTimer(
            TimerHandle_Explode,
            this,
            &AIT_ItemBase_Grenade::Explode,
            FuseTime,
            false
        );
    }
}

void AIT_ItemBase_Grenade::OnGrenadeHit(
    UPrimitiveComponent* HitComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent()))
    {
        FVector Velocity = RootPrim->GetPhysicsLinearVelocity();

        //  속도 감쇠 (무한 튕김 방지)
        Velocity *= 0.8f; // 너무 세면 빠르게 멈춤

        //  바닥에 부딪히면 약간의 수평 튕김 유지
        if (FMath::Abs(Hit.Normal.Z) > 0.6f) // 바닥면일 때
        {
            Velocity.Z *= -0.6f; // 반사 효과
        }

        RootPrim->SetPhysicsLinearVelocity(Velocity);

        if (BounceSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, GetActorLocation());
        }
    }
}

void AIT_ItemBase_Grenade::Explode()
{
    if (!HasAuthority())
    {
        return;
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    }

    Multicast_PlayExplosionFX();

    // 폭발 반경 내 액터 검색 후 피해 적용
    TArray<AActor*> OverlappingActors;
    ExplosionRadiusComp->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

    for (AActor* Target : OverlappingActors)
    {
        UGameplayStatics::ApplyDamage(
            Target,
            ExplosionDamage,
            GetInstigatorController(),
            this,
            nullptr
        );
    }

    Destroy();
}

void AIT_ItemBase_Grenade::Multicast_PlayExplosionFX_Implementation()
{
    if (ExplosionVFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionVFX,
            GetActorLocation(),
            FRotator::ZeroRotator
        );
    }
}
