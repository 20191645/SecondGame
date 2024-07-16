// SCharacter.cpp

#include "Character/SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/SStatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SGameState.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    float CharacterHalfHeight = 95.f;
    float CharacterRadius = 20.f;

    GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);
    // Collision Preset ����
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

    // PivotPosition, PivotRotation: ĸ�� ������Ʈ�� ������ ��ġ, ������ ������
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    // MaxWalkSpeed: ĳ���Ͱ� Walk mode�� �� �ִ� �ӷ�
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    // MinAnalogWalkSpeed: �����а� �Ƴ��α�(Ű���� - ���̽�ƽ) �Է� ������ ������ ���ϰ�
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    // JumpZVelocity: ĳ���Ͱ� ������ �� �ӵ�
    GetCharacterMovement()->JumpZVelocity = 700.f;
    // AirControl: ���߿� ������ �� ������ �� �ִ� ����
    GetCharacterMovement()->AirControl = 0.35f;
    // BrakingDecelerationWalking: �������� ������ �� ���ߴ� �ӵ�
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // 'StatComponent' ������Ʈ ����
    StatComponent = CreateDefaultSubobject<USStatComponent>(TEXT("StatComponent"));
    // 'StatComponent' ���� ���
    StatComponent->SetIsReplicated(true);
}

void ASCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 'OnOutOfCurrentHPDelegate'�� OnCharacterDeath() �Լ� ���ε�
    if (IsValid(StatComponent) == true && StatComponent->OnOutOfCurrentHPDelegate.IsAlreadyBound(this, &ThisClass::OnCharacterDeath) == false)
    {
        StatComponent->OnOutOfCurrentHPDelegate.AddDynamic(this, &ThisClass::OnCharacterDeath);
    }
}

void ASCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 'OnOutOfCurrentHPDelegate'�� OnCharacterDeath() �Լ� ����ε�
    if (true == StatComponent->OnOutOfCurrentHPDelegate.IsAlreadyBound(this, &ThisClass::OnCharacterDeath))
    {
        StatComponent->OnOutOfCurrentHPDelegate.RemoveDynamic(this, &ThisClass::OnCharacterDeath);
    }

    Super::EndPlay(EndPlayReason);
}

float ASCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // ���� ���� �� ���� ���� -- ��Ƽ �÷���
    ASGameState* SGameState = Cast<ASGameState>(UGameplayStatics::GetGameState(this));
    if (IsValid(SGameState) == true && SGameState->MatchState != EMatchState::Playing)
    {
        return FinalDamageAmount;
    }

    // 'CurrentHP' ����
    StatComponent->SetCurrentHP(StatComponent->GetCurrentHP() - FinalDamageAmount);

    return FinalDamageAmount;
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, CurrentAimPitch);
    DOREPLIFETIME(ThisClass, CurrentAimYaw);

    DOREPLIFETIME(ThisClass, HitReactAnimMontage);
}

void ASCharacter::OnCharacterDeath()
{
    // ĳ���� �浹, ������ ����
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    
    // �÷��̾� ĳ���� �Է� �̺�Ʈ ����
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (IsValid(PlayerController)) {
        DisableInput(PlayerController);
    }
}
