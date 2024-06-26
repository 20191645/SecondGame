// SPlayerCharacter.cpp

#include "Character/SPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/SInputConfigData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/SWeaponActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/SAnimInstance.h"

ASPlayerCharacter::ASPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	// 카메라 봉이 회전하면 자식인 카메라도 함께 회전하도록 연결
	CameraComponent->SetupAttachment(SpringArmComponent);

    // bUseControllerRotation***: 컨트롤러 움직임(마우스)에 따라 캐릭터가 회전하지 않는다 [false]
    // -> 마우스 상하좌우 회전에 캐릭터 방향은 바뀌지 않는다
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // bUsePawnControlRotation: 컨트롤러 움직임에 따라 Spring Arm(카메라 지지대)가 같이 회전한다 [true]
    // -> ControlRotation 값과 Spring Arm 회전값이 동기화 => 카메라 방향이 ControlRotation 값
    SpringArmComponent->bUsePawnControlRotation = true;
    // bInherit***: Root Component(Pawn)가 가져오는 Control Rotation 값을 반영한다 [true]
    SpringArmComponent->bInheritPitch = true;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bInheritRoll = false;
    // bDoCollisionTest: 카메라가 벽에 부딪힐 때 뚫고 지나가지 않는다 [true]
    SpringArmComponent->bDoCollisionTest = true;
    SpringArmComponent->SetRelativeLocation(FVector(0.f, 50.f, 50.f));

    // RotationRate: 회전 속도
    GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
    // bOrientRotationToMovement: 캐릭터 이동 방향에 회전을 일치시키지 않는다 [false]
    GetCharacterMovement()->bOrientRotationToMovement = false;
    // bUseControllerDesiredRotation: ControlRotation을 목표 회전값으로 삼아서 회전하지 않는다 [false]
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    // 웅크리기 동작 허용
    if (GetMovementComponent()) {
        GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
    }
}

void ASPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (IsValid(PlayerController) == true)
    {
        // UEnhancedInputLocalPlayerSubsystem: Enhanced Input System의 모든 내용을 관리하는 싱글톤 패턴
        // GetSubSystem('Player'): 플레이어 컨트롤러의 '플레이어(나)'에게 해당하는 서브 시스템을 반환
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (IsValid(Subsystem) == true)
        {
            // 서브 시스템에 Mapping Context 추가
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }
}

void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced Input System을 이용하여 Input Action 바인드 <- Input Config Data 이용
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (IsValid(EnhancedInputComponent) == true)
    {
        // Move('IA_Move')을 트리거 상태에서 InputMove() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
        // Look('IA_Look')을 트리거 상태에서 InputLook() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
        // Jump('IA_Jump')을 스타트 상태에서 ACharater 클래스의 Jump() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
        // QuickSlot01('IA_QuickSlot01')을 스타트 상태에서 InputQuickSlot01() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot01, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot01);
        // QuickSlot02('IA_QuickSlot02')을 스타트 상태에서 InputQuickSlot02() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot02, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot02);
        // QuickSlot03('IA_QuickSlot03')을 스타트 상태에서 InputQuickSlot03() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot03, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot03);
        // Crouch('IA_Crouch')을 스타트 상태에서 InputCrouch() 함수와 바인드
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Crouch, ETriggerEvent::Started, this, &ThisClass::InputCrouch);
    }
}

void ASPlayerCharacter::InputMove(const FInputActionValue& InValue)
{
    // Input Action Value를 FVector2D(2차원) 형태로 해석하여 반환
    FVector2D MovementVector = InValue.Get<FVector2D>();

    // ControlRotation: 플레이어 의지 속성(마우스 움직임)에 따라 폰이 회전해야 할 회전값
    // 플레이어의 회전 의지 중 Yaw 성분으로만 전진 방향 결정 -> 이동 방향은 마우스의 좌우 방향에만 영향 받는다.
    // Pitch 값 안 쓰는 이유 - 마우스가 위를 쳐다본다고 공중으로 이동하면 안되기 때문.
    const FRotator ControlRotation = GetController()->GetControlRotation();
    const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

    // Rotator을 통해 Matrix 생성 
    // -> Matrix의 한 행이 각각 기즈모의 xyz축에 대응 
    // -> GetUnitAxis() 함수 사용하여 각 축을 Vector로 가져옴
    // Rotator: [pitch] [yaw]	[roll]
    // Matrix:	[pitch] [0]		[0]		- x축
    //			[0]		[yaw]	[0]		- y축
    //			[0]		[0]		[roll]	- z축
    const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
    const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

    // 캐릭터 이동
    AddMovementInput(ForwardVector, MovementVector.X);
    AddMovementInput(RightVector, MovementVector.Y);

    // Input Value 속성(Forward, Right) 값 설정
    ForwardInputValue = MovementVector.X;
    RightInputValue = MovementVector.Y;
}

void ASPlayerCharacter::InputLook(const FInputActionValue& InValue)
{
    if (IsValid(GetController()) == true)
    {
        FVector2D LookVector = InValue.Get<FVector2D>();

        // 'IMC_PlayerCharacter'에서 X값에는 마우스 좌우 값을, Y값에는 마우스 상하 값의 반대값을 설정
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
    }
}

void ASPlayerCharacter::InputQuickSlot01(const FInputActionValue& InValue)
{
    if (IsValid(WeaponInstance) == true)
    {
        // 무기 해제 시 사용할 Animation Layer 연결
        TSubclassOf<UAnimInstance> UnarmedCharacterAnimLayer = WeaponInstance->GetUnarmedCharacterAnimLayer();
        if (IsValid(UnarmedCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(UnarmedCharacterAnimLayer);
        }

        // 무기 탈착 애니메이션 재생
        USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
        if (IsValid(AnimInstance) == true && IsValid(WeaponInstance->GetUnequipAnimMontage()))
        {
            AnimInstance->Montage_Play(WeaponInstance->GetUnequipAnimMontage());
        }

        // 장착한 무기 제거
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }
}

void ASPlayerCharacter::InputQuickSlot02(const FInputActionValue& InValue)
{
    if (IsValid(WeaponInstance) == true)
    {
        // 이미 'WeaponClass01' 클래스의 무기 액터 장착 시 return
        if (WeaponInstance.GetClass() == WeaponClass01) {
            return;
        }

        // 'WeaponClass02' 클래스의 무기 액터 장착 시 삭제
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }

    FName WeaponSocket(TEXT("WeaponSocket01"));
    if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
    {
        // 'WeaponClass01' 클래스의 무기 액터 스폰
        WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass01, FVector::ZeroVector, FRotator::ZeroRotator);
        if (IsValid(WeaponInstance) == true)
        {
            WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
        }

        // 피스톨 무기 장착 시 사용할 Animation Layer 연결
        TSubclassOf<UAnimInstance> PistolCharacterAnimLayer = WeaponInstance->GetPistolCharacterAnimLayer();
        if (IsValid(PistolCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(PistolCharacterAnimLayer);
        }

        // 무기 장착 애니메이션 재생
        USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
        if (IsValid(AnimInstance) == true && IsValid(WeaponInstance->GetEquipAnimMontage()))
        {
            AnimInstance->Montage_Play(WeaponInstance->GetEquipAnimMontage());
        }
    }
}

void ASPlayerCharacter::InputQuickSlot03(const FInputActionValue& InValue)
{
    if (IsValid(WeaponInstance) == true)
    {
        // 이미 'WeaponClass02' 클래스의 무기 액터 장착 시 return
        if (WeaponInstance.GetClass() == WeaponClass02) {
            return;
        }

        // 'WeaponClass01' 클래스의 무기 액터 장착 시 삭제
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }

    FName WeaponSocket(TEXT("WeaponSocket02"));
    if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
    {
        // 'WeaponClass02' 클래스의 무기 액터 스폰
        WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass02, FVector::ZeroVector, FRotator::ZeroRotator);
        if (IsValid(WeaponInstance) == true)
        {
            WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
        }

        // 라이플 무기 장착 시 사용할 Animation Layer 연결
        TSubclassOf<UAnimInstance> RifleCharacterAnimLayer = WeaponInstance->GetRifleCharacterAnimLayer();
        if (IsValid(RifleCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(RifleCharacterAnimLayer);
        }

        // 무기 장착 애니메이션 재생
        USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
        if (IsValid(AnimInstance) == true && IsValid(WeaponInstance->GetEquipAnimMontage()))
        {
            AnimInstance->Montage_Play(WeaponInstance->GetEquipAnimMontage());
        }
    }
}

void ASPlayerCharacter::InputCrouch(const FInputActionValue& InValue)
{
    if (bIsCrouched) {
        UnCrouch();

    }
    else {
        Crouch();
    }
}