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
    // Collision Preset 설정
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

    // PivotPosition, PivotRotation: 캡슐 컴포넌트와 에셋의 위치, 방향을 맞춰줌
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    // MaxWalkSpeed: 캐릭터가 Walk mode일 때 최대 속력
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    // MinAnalogWalkSpeed: 디지털과 아날로그(키보드 - 조이스틱) 입력 움직임 간극의 보완값
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    // JumpZVelocity: 캐릭터가 점프할 때 속도
    GetCharacterMovement()->JumpZVelocity = 700.f;
    // AirControl: 공중에 떠있을 때 움직일 수 있는 정도
    GetCharacterMovement()->AirControl = 0.35f;
    // BrakingDecelerationWalking: 움직임을 멈췄을 때 멈추는 속도
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // 'StatComponent' 오브젝트 생성
    StatComponent = CreateDefaultSubobject<USStatComponent>(TEXT("StatComponent"));
    // 'StatComponent' 복제 허용
    StatComponent->SetIsReplicated(true);
}

void ASCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 'OnOutOfCurrentHPDelegate'에 OnCharacterDeath() 함수 바인드
    if (IsValid(StatComponent) == true && StatComponent->OnOutOfCurrentHPDelegate.IsAlreadyBound(this, &ThisClass::OnCharacterDeath) == false)
    {
        StatComponent->OnOutOfCurrentHPDelegate.AddDynamic(this, &ThisClass::OnCharacterDeath);
    }
}

void ASCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 'OnOutOfCurrentHPDelegate'에 OnCharacterDeath() 함수 언바인드
    if (true == StatComponent->OnOutOfCurrentHPDelegate.IsAlreadyBound(this, &ThisClass::OnCharacterDeath))
    {
        StatComponent->OnOutOfCurrentHPDelegate.RemoveDynamic(this, &ThisClass::OnCharacterDeath);
    }

    Super::EndPlay(EndPlayReason);
}

float ASCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 게임 시작 전 무적 상태 -- 멀티 플레이
    ASGameState* SGameState = Cast<ASGameState>(UGameplayStatics::GetGameState(this));
    if (IsValid(SGameState) == true && SGameState->MatchState != EMatchState::Playing)
    {
        return FinalDamageAmount;
    }

    // 'CurrentHP' 수정
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
    // 캐릭터 충돌, 움직임 해제
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    
    // 플레이어 캐릭터 입력 이벤트 막기
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (IsValid(PlayerController)) {
        DisableInput(PlayerController);
    }
}
