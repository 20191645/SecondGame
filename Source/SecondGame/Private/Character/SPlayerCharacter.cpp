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
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"

ASPlayerCharacter::ASPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 30.f, 90.f));
	SpringArmComponent->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	// RotationRate: ȸ�� �ӵ�
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	// bOrientRotationToMovement: ĳ���� �̵� ���⿡ ȸ���� ��ġ��Ű�� �ʴ´� [false]
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// bUseControllerDesiredRotation: ControlRotation�� ��ǥ ȸ�������� ��Ƽ� ȸ������ �ʴ´� [false]
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// �ɴ� ���� ���
	if (GetMovementComponent()) {
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}

	// ���� ��� �ð� ���� �ʱ�ȭ
	TimeBetweenFire = 60.f / FirePerMinute;
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

	// ���� ���� �� �⺻���� 'WeaponSocket01' Ŭ���� ���� ���� ����
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 'WeaponClass01' Ŭ������ ���� ���� ����
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass01, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// �� ���� ��ü ���� �� ȭ�鿡 �Ⱥ��̰� �߰�
	if (IsValid(SniperZoomUIClass) == true) {
		SniperZoomUIInstance = CreateWidget<UUserWidget>(PlayerController, SniperZoomUIClass);
		SniperZoomUIInstance->AddToViewport(0);
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ������ �ӵ��� ���� FOV ���� ��ǥ FOV ������ ����
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);
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
		// Attack('IA_Attack')�� ��ŸƮ ���¿��� InputAttack() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Started, this, &ThisClass::InputAttack);
		// Zoom('IA_Zoom')�� ��ŸƮ ���¿��� ZoomIn() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Zoom, ETriggerEvent::Started, this, &ThisClass::ZoomIn);
		// Zoom('IA_Zoom')�� ���ø� ���¿��� ZoomOut() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Zoom, ETriggerEvent::Completed, this, &ThisClass::ZoomOut);
		// Trigger('IA_Trigger')�� ��ŸƮ ���¿��� ToggleTrigger() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Trigger, ETriggerEvent::Started, this, &ThisClass::ToggleTrigger);
		// Attack('IA_Attack')�� ��ŸƮ ���¿��� StartFire() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Started, this, &ThisClass::StartFire);
		// Attack('IA_Attack')�� ���ø� ���¿��� StopFire() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Completed, this, &ThisClass::StopFire);

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
		// �̹� 'WeaponClass01' Ŭ������ ���� ���� ���� �� return
		if (WeaponInstance.GetClass() == WeaponClass01) {
			return;
		}

		// �ٸ� Ŭ������ ���� ���� ���� �� ����
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;

		// ���� ���� ����
		bIsTriggerToggle = false;
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

void ASPlayerCharacter::InputQuickSlot02(const FInputActionValue& InValue)
{
	if (IsValid(WeaponInstance) == true)
	{
		// �̹� 'WeaponClass02' Ŭ������ ���� ���� ���� �� return
		if (WeaponInstance.GetClass() == WeaponClass02) {
			return;
		}

		// �ٸ� Ŭ������ ���� ���� ���� �� ����
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

void ASPlayerCharacter::InputQuickSlot03(const FInputActionValue& InValue)
{
	if (IsValid(WeaponInstance) == true)
	{
		// �̹� 'WeaponClass03' Ŭ������ ���� ���� ���� �� return
		if (WeaponInstance.GetClass() == WeaponClass03) {
			return;
		}

		// �ٸ� Ŭ������ ���� ���� ���� �� ����
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;

		// ���� ���� ����
		bIsTriggerToggle = false;
	}

	FName WeaponSocket(TEXT("WeaponSocket03"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 'WeaponClass02' Ŭ������ ���� ���� ����
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass03, FVector::ZeroVector, FRotator::ZeroRotator);
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

void ASPlayerCharacter::InputAttack(const FInputActionValue& InValue)
{
	// ĳ���Ͱ� ���� ���̸� ���� �Ұ�
	if (GetCharacterMovement()->IsFalling() == true)
	{
		return;
	}

	// �ܹ� ��� �Լ� ȣ��
	if (bIsTriggerToggle == false) {
		TryFire();
	}
}

void ASPlayerCharacter::TryFire()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (IsValid(PlayerController) == true && IsValid(WeaponInstance) == true) {

#pragma region CaculateTargetTransform
		// FocalDistance: ���� �Ÿ� (SpringArmLength�� ����� ��)
		float FocalDistance = 400.f;
		// FocalLocation: ���� ��ġ
		FVector FocalLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		// �÷��̾� ��Ʈ�ѷ��κ��� ī�޶� ��ġ, ���� ��������
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// ī�޶� ������ ���� ���� ���ͷ� ��ȯ
		FVector AimDirectionFromCamera = CameraRotation.Vector().GetSafeNormal();
		// AimDirection���κ����� ���� ��ġ ���ϱ� (���� ���� ����X)
		FocalLocation = CameraLocation + (AimDirectionFromCamera * FocalDistance);

		// WeaponMuzzleLocation: ���� ������ MuzzleFlash ���� ��ġ
		FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
		// FinalFocalLocation: FocalLocation�� WeaponMuzzleLocation �ݿ��� ���� ���� ��ġ
		// -- (WeaponMuzzleLocation - FocalLocation): FocalLocation���� WeaponMuzzleLocation���� ���� ���� ����
		// -- ((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera): ���� ���� -> ��Į�� �� [����]
		FVector FinalFocalLocation = FocalLocation + (((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera) * AimDirectionFromCamera);

		// TargetTransform: ��� ���� ����
		FTransform TargetTransform = FTransform(CameraRotation, FinalFocalLocation);
#pragma endregion

#pragma region LineTracing
		// BulletDirection: ��� ���� -- TargetTransform�� Forward ����
		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		// StartLocation: ��� ������ ��ġ (=FinalFocalLocation)
		FVector StartLocation = TargetTransform.GetLocation();
		// EndLocation: (��� ������ + ��Ÿ�) ��ġ
		FVector EndLocation = StartLocation + BulletDirection * WeaponInstance->GetMaxRange();

		// HitResult: �浹 Ž�� ���
		FHitResult HitResult;
		// TraceParams: �浹 Ž�� ���
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		// �浹 Ž�� ��󿡼� ���� ���ʹ� ����
		TraceParams.AddIgnoredActor(WeaponInstance);

		// LineTraceSingleByChannel: StartLocation���� EndLocation ������ ��ġ���� TraceParams �� 'Attack' ä���� ����Ǵ� �浹 Ž�� ����� HitResult�� ����
		bool IsCollided = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams);
		// �浹 ���� ���� �� TraceStart, TraceEnd �� ���� ����
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		// �浹 Ž�� �Ÿ� ����� ����� <- ���� �Ϸ� �� �ּ� ó��
		if (IsCollided == true)
		{
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 60.f, 0, 2.f);
		}
		else
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 60.f, 0, 2.f);
		}
#pragma endregion

#pragma region DamageEvent
		// ��� ������ ĳ���� ������ ������
		if (IsCollided == true)
		{
			ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;

				// ���� Ŭ�������� ������ ���� �α�
				if (WeaponInstance.GetClass() == WeaponClass01) {
					HittedCharacter->TakeDamage(5.f, DamageEvent, GetController(), this);
				}
				else if (WeaponInstance.GetClass() == WeaponClass02) {
					HittedCharacter->TakeDamage(10.f, DamageEvent, GetController(), this);
				}
				else {
					HittedCharacter->TakeDamage(20.f, DamageEvent, GetController(), this);
				}
			}
		}
#pragma endregion
	}
}

void ASPlayerCharacter::ZoomIn(const FInputActionValue& InValue)
{
	// 'WeaponClass03' Ŭ���� ���Ⱑ �ƴϸ� ���� ����
	if (WeaponInstance.GetClass() != WeaponClass03) {
		return;
	}

	// ��ǥ FOV �� ���� -> ����
	TargetFOV = 30.f;

	// �� ���� ȭ�鿡 ���̰� ����
	if (IsValid(SniperZoomUIInstance) == true)
	{
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASPlayerCharacter::ZoomOut(const FInputActionValue& InValue)
{
	// ��ǥ FOV �� ���� -> �ܾƿ�
	TargetFOV = 70.f;

	// �� ���� ȭ�鿡�� �Ⱥ��̰� ����
	if (IsValid(SniperZoomUIInstance) == true)
	{
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASPlayerCharacter::ToggleTrigger(const FInputActionValue& InValue)
{
	// 'WeaponClass02' Ŭ���� ���Ⱑ �ƴϸ� ���� ����
	if (WeaponInstance.GetClass() != WeaponClass02) {
		return;
	}

	// �Ӽ��� ����
	bIsTriggerToggle = !bIsTriggerToggle;
}

void ASPlayerCharacter::StartFire(const FInputActionValue& InValue)
{
	// ���� ��� Ÿ�̸� ����
	if (bIsTriggerToggle == true)
	{
		GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::TryFire, TimeBetweenFire, true);
	}
}

void ASPlayerCharacter::StopFire(const FInputActionValue& InValue)
{
	// ���� ��� Ÿ�̸� Ŭ����
	GetWorldTimerManager().ClearTimer(BetweenShotsTimer);
}
