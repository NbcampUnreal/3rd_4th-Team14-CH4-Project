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
    // MeshComp : ���� ����
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    SetRootComponent(MeshComp);

    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetEnableGravity(true);
    MeshComp->SetNotifyRigidBodyCollision(true);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
    MeshComp->OnComponentHit.AddDynamic(this, &AIT_ItemBase_Grenade::OnGrenadeHit);

    // ����(���� ���� �� ȸ�� ����)
    MeshComp->SetLinearDamping(0.1f);   // ���� ����: ���� ������ �ڿ� ����
    MeshComp->SetAngularDamping(0.1f);   // ȸ�� ����: �ٴڿ��� ������ ����
    MeshComp->SetMassOverrideInKg(NAME_None, 2.5f, true); // ���԰� �߰�

    // ���� ���� ����
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

    // ���� ��Ƽ���� ���� (BeginPlay �������� ����)
    UPhysicalMaterial* PhysMat = NewObject<UPhysicalMaterial>(this);
    PhysMat->Friction = 0.4f;       // ������: �ణ ���缭 �� ��������
    PhysMat->Restitution = 0.8f;    // �ݹ߷�: ƨ�� ������ ���� ����
    MeshComp->SetPhysMaterialOverride(PhysMat);

    // ������ ���� Ÿ�̸� ����
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

        //  �ӵ� ���� (���� ƨ�� ����)
        Velocity *= 0.8f; // �ʹ� ���� ������ ����

        //  �ٴڿ� �ε����� �ణ�� ���� ƨ�� ����
        if (FMath::Abs(Hit.Normal.Z) > 0.6f) // �ٴڸ��� ��
        {
            Velocity.Z *= -0.6f; // �ݻ� ȿ��
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

    // ���� �ݰ� �� ���� �˻� �� ���� ����
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
