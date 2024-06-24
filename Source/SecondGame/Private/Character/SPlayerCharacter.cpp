// SPlayerCharacter.cpp

#include "Character/SPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/SInputConfigData.h"
#include "GameFramework/CharacterMovementComponent.h"

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
        // Move('IA_Move')�� Ʈ���� ���¿��� InputMove() �Լ��� ���ε� �����ش�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
        // Look('IA_Look')�� Ʈ���� ���¿��� InputLook() �Լ��� ���ε� �����ش�
        EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
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
