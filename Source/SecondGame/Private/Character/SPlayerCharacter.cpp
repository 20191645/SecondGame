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
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Component/SStatComponent.h"
#include "Game/SPlayerState.h"
#include "Particles/ParticleSystemComponent.h"
#include "Game/SGameMode.h"

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

	// ParticleSystemComponent ������Ʈ �Ҵ�
	RespawnParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RespawnParticleSystemComponent"));
	RespawnParticleSystemComponent->SetupAttachment(GetRootComponent());
	// Particle�� ��Ʈ���� �ʴ´� [false]
	RespawnParticleSystemComponent->SetAutoActivate(false);

	// ���� ���� Ŭ���� ��ȣ ����
	SetWeaponClassNumber(1);

	// �Ѿ� ���� �ʱ�ȭ
	MaxBulletCount = { 15,30,5 };
	CurrentBulletCount.SetNum(3);
	for (int i = 1; i <= 3; i++) {
		SetCurrentBulletCount(i, MaxBulletCount[i - 1]);
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

	// ���� ���� �� 'WeaponSocket'�� 'WeaponClass01' Ŭ���� ���� ���� ����
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
	if (IsValid(PlayerController) == true && IsValid(SniperZoomUIClass) == true) {
		SniperZoomUIInstance = CreateWidget<UUserWidget>(PlayerController, SniperZoomUIClass);
		if (IsValid(SniperZoomUIInstance) == true) {
			SniperZoomUIInstance->AddToViewport();
			SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 'OnFireEffect' ��������Ʈ�� OnFireEffect() �Լ� ���ε�
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true)
	{
		AnimInstance->OnFireEffect.AddDynamic(this, &ThisClass::OnFireEffect);
	}

	// ��� ���� ȿ�� ���� ��ü ���� �� ȭ�鿡 �Ⱥ��̰� �߰�
	if (IsValid(PlayerController) == true && IsValid(FireHitEffectUIClass) == true) {
		FireHitEffectUIInstance = CreateWidget<UUserWidget>(PlayerController, FireHitEffectUIClass);
		if (IsValid(SniperZoomUIInstance) == true) {
			FireHitEffectUIInstance->AddToViewport();
			FireHitEffectUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ASPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ������ �ӵ��� ���� FOV ���� ��ǥ FOV ������ ����
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);

	// ��Ʈ�� �����̼����κ��� ���� ���� �� �ǽð� ������Ʈ
	if (IsValid(GetController()) == true)
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		CurrentAimPitch = ControlRotation.Pitch;
		CurrentAimYaw = ControlRotation.Yaw;
	}
}

void ASPlayerCharacter::OnFireEffect()
{
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == true)
	{
		FRotator ParticleRotation(75.f, 0.f, 0.f);
		// ���� Ŭ������ ���� WeaponSocket���� �ѱ������� �Ÿ� ����
		FVector ParticleLocation;
		switch (WeaponClassNumber) {
		case 1:
			ParticleLocation = FVector(-10.f, 20.f, 5.f);
			break;
		case 2:
			ParticleLocation = FVector(-15.f, 65.f, 20.f);
			break;
		case 3:
			ParticleLocation = FVector(-10.f, 70.f, 20.f);
			break;
		}
		
		// 'FireParticleSystem' ȿ�� ���
		if (IsValid(WeaponInstance->GetFireParticleSystem())) {
			UGameplayStatics::SpawnEmitterAttached(
				WeaponInstance->GetFireParticleSystem(),
				GetMesh(),
				WeaponSocket,
				ParticleLocation,
				ParticleRotation,
				(FVector)(0.3f)
			);
		}
	}
}

float ASPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinialDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// �÷��̾� ĳ���� ����
	if (GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		// �÷��̾� ĳ���͸� ���� �÷��̾��� 'CurrentKillCount' �� ����
		// -> NPC ĳ���Ϳ��� ������ ����X
		ASPlayerCharacter* DamageCauserCharacter = Cast<ASPlayerCharacter>(DamageCauser);
		if (IsValid(DamageCauserCharacter) == true)
		{
			ASPlayerState* DCCPlayerState = Cast<ASPlayerState>(DamageCauserCharacter->GetPlayerState());
			if (IsValid(DCCPlayerState) == true)
			{
				DCCPlayerState->AddCurrentKillCount(1);
			}
		}
	}
	// �ǰ� ����
	else {
		// �ǰ� �ִϸ��̼� ���
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance) && IsValid(HitReactAnimMontage))
		{
			AnimInstance->Montage_Play(HitReactAnimMontage);
		}
	}

	return FinialDamageAmount;
}

void ASPlayerCharacter::SetCurrentBulletCount(int32 InWeaponClassNumber, int32 InCurrentBulletCount)
{
	if (true == OnCurrentBulletCountChangedDelegate.IsBound())
	{
		OnCurrentBulletCountChangedDelegate.Broadcast(InWeaponClassNumber, InCurrentBulletCount);
	}

	CurrentBulletCount[InWeaponClassNumber - 1] =
		FMath::Clamp<int32>(InCurrentBulletCount, 0, MaxBulletCount[InWeaponClassNumber - 1]);
}

void ASPlayerCharacter::SetWeaponClassNumber(int32 InWeaponClassNumber)
{
	if (true == OnWeaponClassNumberChangedDelegate.IsBound())
	{
		OnWeaponClassNumberChangedDelegate.Broadcast(InWeaponClassNumber);
	}

	WeaponClassNumber = FMath::Clamp<int32>(InWeaponClassNumber, 1, 3);
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
		// Reload('IA_Reload')�� ��ŸƮ ���¿��� InputReload() �Լ��� ���ε�
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Reload, ETriggerEvent::Started, this, &ThisClass::InputReload);
	}
}

void ASPlayerCharacter::OnCharacterDeath()
{
	// 'SCharacter' Ŭ������ OnCharacterDeath() �Լ� ����
	Super::OnCharacterDeath();

	// �÷��̾��� 'CurrentDeathCount' �� ����
	// -> ���� ���� ������� ������ ����
	ASPlayerState* SPlayerState = Cast<ASPlayerState>(GetPlayerState());
	if (IsValid(SPlayerState) == true)
	{
		SPlayerState->AddCurrentDeathCount(1);
	}

	// �ܾƿ�
	ZoomOut();

	// ���� ���� ���� �ִ� ���� ������ ������ 5�� �� ������
	if (IsValid(SPlayerState) == true)
	{
		if (SPlayerState->GetCurrentDeathCount() < SPlayerState->GetMaxDeathCount()) {
			FTimerHandle respawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(respawnTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				Respawn();
			}), 5.0f, false);
		}
	}
}

void ASPlayerCharacter::Respawn()
{
	// ĳ���� �浹, ������ ����
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// ĳ���� HP ȸ��
	GetStatComponent()->SetCurrentHP(GetStatComponent()->GetMaxHP());

	// ĳ���� ���� Ŭ���� �ʱ�ȭ
	InputQuickSlot01();

	// ĳ���� �Ѿ� ȸ��
	for (int i = 1; i <= 3; i++) {
		SetCurrentBulletCount(i, MaxBulletCount[i - 1]);
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController)) {
		// ĳ���� �Է� ���
		EnableInput(PlayerController);

		// ĳ���� ��ġ �̵�
		AGameModeBase* MyMode = Cast<AGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(MyMode) == true) {
			// 'PlayerStartTag' �̸�
			FString PlayerStartTag = "Player0";
			AActor* StartSpot = MyMode->FindPlayerStart(PlayerController, PlayerStartTag);
			SetActorLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
		}
	}

	// ĳ���� ��Ȱ ����Ʈ ����
	RespawnParticleSystemComponent->ActivateSystem(true);

	// ĳ���� 3�ʰ� ���� ����
	SetCanBeDamaged(false);
	FTimerHandle damagedTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(damagedTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		SetCanBeDamaged(true);
	}), 3.0f, false);
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

void ASPlayerCharacter::InputQuickSlot01()
{
	// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
		return;
	}
	
	if (IsValid(WeaponInstance) == true)
	{
		// �̹� 'WeaponClass01' Ŭ������ ���� ���� ���� �� return
		if (WeaponClassNumber == 1) {
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
		if (IsValid(WeaponInstance->GetEquipAnimMontage()))
		{
			AnimInstance->Montage_Play(WeaponInstance->GetEquipAnimMontage());
		}

		// ���� ���� Ŭ���� ��ȣ ����
		SetWeaponClassNumber(1);
	}
}

void ASPlayerCharacter::InputQuickSlot02()
{
	// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
		return;
	}

	if (IsValid(WeaponInstance) == true)
	{
		// �̹� 'WeaponClass02' Ŭ������ ���� ���� ���� �� return
		if (WeaponClassNumber == 2) {
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
		if (IsValid(WeaponInstance->GetEquipAnimMontage()))
		{
			AnimInstance->Montage_Play(WeaponInstance->GetEquipAnimMontage());
		}

		// ���� ���� Ŭ���� ��ȣ ����
		SetWeaponClassNumber(2);
	}
}

void ASPlayerCharacter::InputQuickSlot03()
{
	// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
		return;
	}

	if (IsValid(WeaponInstance) == true)
	{
		// �̹� 'WeaponClass03' Ŭ������ ���� ���� ���� �� return
		if (WeaponClassNumber == 3) {
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
		if (IsValid(WeaponInstance->GetEquipAnimMontage()))
		{
			AnimInstance->Montage_Play(WeaponInstance->GetEquipAnimMontage());
		}

		// ���� ���� Ŭ���� ��ȣ ����
		SetWeaponClassNumber(3);
	}
}

void ASPlayerCharacter::InputCrouch()
{
	if (bIsCrouched) {
		UnCrouch();

	}
	else {
		Crouch();
	}
}

void ASPlayerCharacter::InputAttack()
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

#pragma region BulletCount
		// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance) == false ||
			AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
			 return;
		}
		
		// �Ѿ� ���� Ȯ��
		if (CurrentBulletCount[WeaponClassNumber - 1] == 0) {
			return;
		}
		// �Ѿ� ���
		SetCurrentBulletCount(WeaponClassNumber, CurrentBulletCount[WeaponClassNumber - 1] - 1);
#pragma endregion

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

		/*
			// �浹 Ž�� �Ÿ� ����� �����
			if (IsCollided == true)
			{
				DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Blue, false, 60.f, 0, 2.f);
			}
			else
			{
				DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 60.f, 0, 2.f);
			}
		*/
#pragma endregion

#pragma region DamageEvent
		// ��� ������ ĳ���� ������ ������
		if (IsCollided == true)
		{
			ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;

				// ��� ������ ĳ���Ϳ��� ���������� ����� ������
				float HitDamage = 0.f;
				// ���� Ŭ�������� ������ ���� �α�
				switch (WeaponClassNumber) {
				case 1: HitDamage = 10.f; break;
				case 2: HitDamage = 5.f; break;
				case 3:
					// ���� ���� ������: 30
					if (SniperZoomUIInstance->GetVisibility() == ESlateVisibility::Visible) {
						HitDamage = 30.f;
					}
					// �ܾƿ� ���� ������: 5
					else {
						HitDamage = 5.f;
					}
					break;
				}

				// ��弦 ���� �� ������ 2�� ����
				FString BoneNameString = HitResult.BoneName.ToString();
				if (true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
				{
					HitDamage *= 2;
				}
				
				HittedCharacter->TakeDamage(HitDamage, DamageEvent, GetController(), this);

				// ��� ���� ȿ�� ���� ȭ�鿡 ���̱�
				FireHitEffectUIInstance->SetVisibility(ESlateVisibility::Visible);
				// 0.5�� �� ȭ�鿡�� �����
				FTimerHandle hitTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(hitTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					FireHitEffectUIInstance->SetVisibility(ESlateVisibility::Collapsed);
				}), 0.4f, false);
			}
		}
#pragma endregion

#pragma region Fire
		// ��� �ִϸ��̼� ���
		if (IsValid(WeaponInstance->GetFireAnimMontage()))
		{
			AnimInstance->Montage_Play(WeaponInstance->GetFireAnimMontage());
		}

		// ī�޶� ��鸲 ����
		if (IsValid(FireCameraShake)) {
			PlayerController->ClientStartCameraShake(FireCameraShake);
		}
#pragma endregion

	}
}

void ASPlayerCharacter::ZoomIn()
{
	// 'WeaponClass03' Ŭ���� ���Ⱑ �ƴϸ� ���� ����
	if (WeaponClassNumber != 3) {
		return;
	}

	// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
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

void ASPlayerCharacter::ZoomOut()
{
	// ��ǥ FOV �� ���� -> �ܾƿ�
	TargetFOV = 70.f;

	// �� ���� ȭ�鿡�� �Ⱥ��̰� ����
	if (IsValid(SniperZoomUIInstance) == true)
	{
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASPlayerCharacter::ToggleTrigger()
{
	// 'WeaponClass02' Ŭ���� ���Ⱑ �ƴϸ� ���� ����
	if (WeaponClassNumber != 2) {
		return;
	}

	// �Ӽ��� ����
	bIsTriggerToggle = !bIsTriggerToggle;
}

void ASPlayerCharacter::StartFire()
{
	// ���� ��� Ÿ�̸� ����
	if (bIsTriggerToggle == true)
	{
		GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::TryFire, TimeBetweenFire, true);
	}
}

void ASPlayerCharacter::StopFire()
{
	// ���� ��� Ÿ�̸� Ŭ����
	GetWorldTimerManager().ClearTimer(BetweenShotsTimer);
}

void ASPlayerCharacter::InputReload()
{
	// �ܾƿ�
	ZoomOut();

	// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
		return;
	}

	// �Ѿ� ���� �ִϸ��̼� ���
	if (IsValid(WeaponInstance->GetReloadAnimMontage()) == true)
	{
		AnimInstance->Montage_Play(WeaponInstance->GetReloadAnimMontage());
	}

	// �Ѿ� ���� ����
	SetCurrentBulletCount(WeaponClassNumber, MaxBulletCount[WeaponClassNumber - 1]);
}