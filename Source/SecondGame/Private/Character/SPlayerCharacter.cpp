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
	// ī�޶� ���� ȸ���ϸ� �ڽ��� ī�޶� �Բ� ȸ���ϵ��� ����
	CameraComponent->SetupAttachment(SpringArmComponent);

    // bUseControllerRotation***: ��Ʈ�ѷ� ������(���콺)�� ���� ĳ���Ͱ� ȸ������ �ʴ´� [false]
    // -> ���콺 �����¿� ȸ���� ĳ���� ������ �ٲ��� �ʴ´�
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // bUsePawnControlRotation: ��Ʈ�ѷ� �����ӿ� ���� Spring Arm(ī�޶� ������)�� ���� ȸ���Ѵ� [true]
    // -> ControlRotation ���� Spring Arm ȸ������ ����ȭ => ī�޶� ������ ControlRotation ��
    SpringArmComponent->bUsePawnControlRotation = true;
    // bInherit***: Root Component(Pawn)�� �������� Control Rotation ���� �ݿ��Ѵ� [true]
    SpringArmComponent->bInheritPitch = true;
    SpringArmComponent->bInheritYaw = true;
    SpringArmComponent->bInheritRoll = false;
    // bDoCollisionTest: ī�޶� ���� �ε��� �� �հ� �������� �ʴ´� [true]
    SpringArmComponent->bDoCollisionTest = true;
    SpringArmComponent->SetRelativeLocation(FVector(0.f, 50.f, 50.f));

    // RotationRate: ȸ�� �ӵ�
    GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
    // bOrientRotationToMovement: ĳ���� �̵� ���⿡ ȸ���� ��ġ��Ű�� �ʴ´� [false]
    GetCharacterMovement()->bOrientRotationToMovement = false;
    // bUseControllerDesiredRotation: ControlRotation�� ��ǥ ȸ�������� ��Ƽ� ȸ������ �ʴ´� [false]
    GetCharacterMovement()->bUseControllerDesiredRotation = false;

    // ��ũ���� ���� ���
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
        // UEnhancedInputLocalPlayerSubsystem: Enhanced Input System�� ��� ������ �����ϴ� �̱��� ����
        // GetSubSystem('Player'): �÷��̾� ��Ʈ�ѷ��� '�÷��̾�(��)'���� �ش��ϴ� ���� �ý����� ��ȯ
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (IsValid(Subsystem) == true)
        {
            // ���� �ý��ۿ� Mapping Context �߰�
            Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
        }
    }
}

void ASPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced Input System�� �̿��Ͽ� Input Action ���ε� <- Input Config Data �̿�
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (IsValid(EnhancedInputComponent) == true)
    {
        // Move('IA_Move')�� Ʈ���� ���¿��� InputMove() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
        // Look('IA_Look')�� Ʈ���� ���¿��� InputLook() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
        // Jump('IA_Jump')�� ��ŸƮ ���¿��� ACharater Ŭ������ Jump() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
        // QuickSlot01('IA_QuickSlot01')�� ��ŸƮ ���¿��� InputQuickSlot01() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot01, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot01);
        // QuickSlot02('IA_QuickSlot02')�� ��ŸƮ ���¿��� InputQuickSlot02() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot02, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot02);
        // QuickSlot03('IA_QuickSlot03')�� ��ŸƮ ���¿��� InputQuickSlot03() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->QuickSlot03, ETriggerEvent::Started, this, &ThisClass::InputQuickSlot03);
        // Crouch('IA_Crouch')�� ��ŸƮ ���¿��� InputCrouch() �Լ��� ���ε�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Crouch, ETriggerEvent::Started, this, &ThisClass::InputCrouch);
    }
}

void ASPlayerCharacter::InputMove(const FInputActionValue& InValue)
{
    // Input Action Value�� FVector2D(2����) ���·� �ؼ��Ͽ� ��ȯ
    FVector2D MovementVector = InValue.Get<FVector2D>();

    // ControlRotation: �÷��̾� ���� �Ӽ�(���콺 ������)�� ���� ���� ȸ���ؾ� �� ȸ����
    // �÷��̾��� ȸ�� ���� �� Yaw �������θ� ���� ���� ���� -> �̵� ������ ���콺�� �¿� ���⿡�� ���� �޴´�.
    // Pitch �� �� ���� ���� - ���콺�� ���� �Ĵٺ��ٰ� �������� �̵��ϸ� �ȵǱ� ����.
    const FRotator ControlRotation = GetController()->GetControlRotation();
    const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

    // Rotator�� ���� Matrix ���� 
    // -> Matrix�� �� ���� ���� ������� xyz�࿡ ���� 
    // -> GetUnitAxis() �Լ� ����Ͽ� �� ���� Vector�� ������
    // Rotator: [pitch] [yaw]	[roll]
    // Matrix:	[pitch] [0]		[0]		- x��
    //			[0]		[yaw]	[0]		- y��
    //			[0]		[0]		[roll]	- z��
    const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
    const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

    // ĳ���� �̵�
    AddMovementInput(ForwardVector, MovementVector.X);
    AddMovementInput(RightVector, MovementVector.Y);

    // Input Value �Ӽ�(Forward, Right) �� ����
    ForwardInputValue = MovementVector.X;
    RightInputValue = MovementVector.Y;
}

void ASPlayerCharacter::InputLook(const FInputActionValue& InValue)
{
    if (IsValid(GetController()) == true)
    {
        FVector2D LookVector = InValue.Get<FVector2D>();

        // 'IMC_PlayerCharacter'���� X������ ���콺 �¿� ����, Y������ ���콺 ���� ���� �ݴ밪�� ����
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
    }
}

void ASPlayerCharacter::InputQuickSlot01(const FInputActionValue& InValue)
{
    if (IsValid(WeaponInstance) == true)
    {
        // ���� ���� �� ����� Animation Layer ����
        TSubclassOf<UAnimInstance> UnarmedCharacterAnimLayer = WeaponInstance->GetUnarmedCharacterAnimLayer();
        if (IsValid(UnarmedCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(UnarmedCharacterAnimLayer);
        }

        // ���� Ż�� �ִϸ��̼� ���
        USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
        if (IsValid(AnimInstance) == true && IsValid(WeaponInstance->GetUnequipAnimMontage()))
        {
            AnimInstance->Montage_Play(WeaponInstance->GetUnequipAnimMontage());
        }

        // ������ ���� ����
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }
}

void ASPlayerCharacter::InputQuickSlot02(const FInputActionValue& InValue)
{
    if (IsValid(WeaponInstance) == true)
    {
        // �̹� 'WeaponClass01' Ŭ������ ���� ���� ���� �� return
        if (WeaponInstance.GetClass() == WeaponClass01) {
            return;
        }

        // 'WeaponClass02' Ŭ������ ���� ���� ���� �� ����
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }

    FName WeaponSocket(TEXT("WeaponSocket01"));
    if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
    {
        // 'WeaponClass01' Ŭ������ ���� ���� ����
        WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass01, FVector::ZeroVector, FRotator::ZeroRotator);
        if (IsValid(WeaponInstance) == true)
        {
            WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
        }

        // �ǽ��� ���� ���� �� ����� Animation Layer ����
        TSubclassOf<UAnimInstance> PistolCharacterAnimLayer = WeaponInstance->GetPistolCharacterAnimLayer();
        if (IsValid(PistolCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(PistolCharacterAnimLayer);
        }

        // ���� ���� �ִϸ��̼� ���
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
        // �̹� 'WeaponClass02' Ŭ������ ���� ���� ���� �� return
        if (WeaponInstance.GetClass() == WeaponClass02) {
            return;
        }

        // 'WeaponClass01' Ŭ������ ���� ���� ���� �� ����
        WeaponInstance->Destroy();
        WeaponInstance = nullptr;
    }

    FName WeaponSocket(TEXT("WeaponSocket02"));
    if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
    {
        // 'WeaponClass02' Ŭ������ ���� ���� ����
        WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass02, FVector::ZeroVector, FRotator::ZeroRotator);
        if (IsValid(WeaponInstance) == true)
        {
            WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
        }

        // ������ ���� ���� �� ����� Animation Layer ����
        TSubclassOf<UAnimInstance> RifleCharacterAnimLayer = WeaponInstance->GetRifleCharacterAnimLayer();
        if (IsValid(RifleCharacterAnimLayer) == true)
        {
            GetMesh()->LinkAnimClassLayers(RifleCharacterAnimLayer);
        }

        // ���� ���� �ִϸ��̼� ���
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