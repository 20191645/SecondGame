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
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 30.f, 90.f));
	SpringArmComponent->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	// RotationRate: 회전 속도
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	// bOrientRotationToMovement: 캐릭터 이동 방향에 회전을 일치시키지 않는다 [false]
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// bUseControllerDesiredRotation: ControlRotation을 목표 회전값으로 삼아서 회전하지 않는다 [false]
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 앉는 동작 허용
	if (GetMovementComponent()) {
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}

	// 연발 사격 시간 간격 초기화
	TimeBetweenFire = 60.f / FirePerMinute;
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

	// 게임 시작 시 기본으로 'WeaponSocket01' 클래스 무기 액터 장착
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 'WeaponClass01' 클래스의 무기 액터 스폰
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass01, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// 줌 위젯 개체 생성 후 화면에 안보이게 추가
	if (IsValid(SniperZoomUIClass) == true) {
		SniperZoomUIInstance = CreateWidget<UUserWidget>(PlayerController, SniperZoomUIClass);
		SniperZoomUIInstance->AddToViewport(0);
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 일정한 속도로 현재 FOV 값을 목표 FOV 값으로 변경
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);
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
		// Attack('IA_Attack')을 스타트 상태에서 InputAttack() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Started, this, &ThisClass::InputAttack);
		// Zoom('IA_Zoom')을 스타트 상태에서 ZoomIn() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Zoom, ETriggerEvent::Started, this, &ThisClass::ZoomIn);
		// Zoom('IA_Zoom')을 컴플릿 상태에서 ZoomOut() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Zoom, ETriggerEvent::Completed, this, &ThisClass::ZoomOut);
		// Trigger('IA_Trigger')을 스타트 상태에서 ToggleTrigger() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Trigger, ETriggerEvent::Started, this, &ThisClass::ToggleTrigger);
		// Attack('IA_Attack')을 스타트 상태에서 StartFire() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Started, this, &ThisClass::StartFire);
		// Attack('IA_Attack')을 컴플릿 상태에서 StopFire() 함수와 바인드
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->Attack, ETriggerEvent::Completed, this, &ThisClass::StopFire);

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
		// 이미 'WeaponClass01' 클래스의 무기 액터 장착 시 return
		if (WeaponInstance.GetClass() == WeaponClass01) {
			return;
		}

		// 다른 클래스의 무기 액터 장착 시 삭제
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;

		// 연발 상태 해제
		bIsTriggerToggle = false;
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

void ASPlayerCharacter::InputQuickSlot02(const FInputActionValue& InValue)
{
	if (IsValid(WeaponInstance) == true)
	{
		// 이미 'WeaponClass02' 클래스의 무기 액터 장착 시 return
		if (WeaponInstance.GetClass() == WeaponClass02) {
			return;
		}

		// 다른 클래스의 무기 액터 장착 시 삭제
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

void ASPlayerCharacter::InputQuickSlot03(const FInputActionValue& InValue)
{
	if (IsValid(WeaponInstance) == true)
	{
		// 이미 'WeaponClass03' 클래스의 무기 액터 장착 시 return
		if (WeaponInstance.GetClass() == WeaponClass03) {
			return;
		}

		// 다른 클래스의 무기 액터 장착 시 삭제
		WeaponInstance->Destroy();
		WeaponInstance = nullptr;

		// 연발 상태 해제
		bIsTriggerToggle = false;
	}

	FName WeaponSocket(TEXT("WeaponSocket03"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 'WeaponClass02' 클래스의 무기 액터 스폰
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass03, FVector::ZeroVector, FRotator::ZeroRotator);
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

void ASPlayerCharacter::InputAttack(const FInputActionValue& InValue)
{
	// 캐릭터가 점프 중이면 공격 불가
	if (GetCharacterMovement()->IsFalling() == true)
	{
		return;
	}

	// 단발 사격 함수 호출
	if (bIsTriggerToggle == false) {
		TryFire();
	}
}

void ASPlayerCharacter::TryFire()
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (IsValid(PlayerController) == true && IsValid(WeaponInstance) == true) {

#pragma region CaculateTargetTransform
		// FocalDistance: 초점 거리 (SpringArmLength와 비슷한 값)
		float FocalDistance = 400.f;
		// FocalLocation: 초점 위치
		FVector FocalLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		// 플레이어 컨트롤러로부터 카메라 위치, 방향 가져오기
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 카메라 방향을 단위 방향 벡터로 변환
		FVector AimDirectionFromCamera = CameraRotation.Vector().GetSafeNormal();
		// AimDirection으로부터의 초점 위치 구하기 (무기 정보 포함X)
		FocalLocation = CameraLocation + (AimDirectionFromCamera * FocalDistance);

		// WeaponMuzzleLocation: 무기 에셋의 MuzzleFlash 소켓 위치
		FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
		// FinalFocalLocation: FocalLocation에 WeaponMuzzleLocation 반영한 최종 초점 위치
		// -- (WeaponMuzzleLocation - FocalLocation): FocalLocation에서 WeaponMuzzleLocation으로 가는 방향 벡터
		// -- ((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera): 벡터 내적 -> 스칼라 값 [길이]
		FVector FinalFocalLocation = FocalLocation + (((WeaponMuzzleLocation - FocalLocation) | AimDirectionFromCamera) * AimDirectionFromCamera);

		// TargetTransform: 사격 시작 지점
		FTransform TargetTransform = FTransform(CameraRotation, FinalFocalLocation);
#pragma endregion

#pragma region LineTracing
		// BulletDirection: 사격 방향 -- TargetTransform의 Forward 벡터
		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		// StartLocation: 사격 시작점 위치 (=FinalFocalLocation)
		FVector StartLocation = TargetTransform.GetLocation();
		// EndLocation: (사격 시작점 + 사거리) 위치
		FVector EndLocation = StartLocation + BulletDirection * WeaponInstance->GetMaxRange();

		// HitResult: 충돌 탐지 결과
		FHitResult HitResult;
		// TraceParams: 충돌 탐지 대상
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		// 충돌 탐지 대상에서 무기 액터는 제외
		TraceParams.AddIgnoredActor(WeaponInstance);

		// LineTraceSingleByChannel: StartLocation에서 EndLocation 사이의 위치에서 TraceParams 중 'Attack' 채널이 적용되는 충돌 탐지 결과를 HitResult에 저장
		bool IsCollided = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams);
		// 충돌 감지 실패 시 TraceStart, TraceEnd 값 직접 지정
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		// 충돌 탐지 거리 디버그 드로잉 <- 개발 완료 시 주석 처리
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
		// 사격 적중한 캐릭터 데미지 입히기
		if (IsCollided == true)
		{
			ASCharacter* HittedCharacter = Cast<ASCharacter>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;

				// 무기 클래스별로 데미지 차이 두기
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
	// 'WeaponClass03' 클래스 무기가 아니면 줌인 막기
	if (WeaponInstance.GetClass() != WeaponClass03) {
		return;
	}

	// 목표 FOV 값 감소 -> 줌인
	TargetFOV = 30.f;

	// 줌 위젯 화면에 보이게 수정
	if (IsValid(SniperZoomUIInstance) == true)
	{
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ASPlayerCharacter::ZoomOut(const FInputActionValue& InValue)
{
	// 목표 FOV 값 증가 -> 줌아웃
	TargetFOV = 70.f;

	// 줌 위젯 화면에서 안보이게 수정
	if (IsValid(SniperZoomUIInstance) == true)
	{
		SniperZoomUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ASPlayerCharacter::ToggleTrigger(const FInputActionValue& InValue)
{
	// 'WeaponClass02' 클래스 무기가 아니면 연발 막기
	if (WeaponInstance.GetClass() != WeaponClass02) {
		return;
	}

	// 속성값 반전
	bIsTriggerToggle = !bIsTriggerToggle;
}

void ASPlayerCharacter::StartFire(const FInputActionValue& InValue)
{
	// 연발 사격 타이머 세팅
	if (bIsTriggerToggle == true)
	{
		GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::TryFire, TimeBetweenFire, true);
	}
}

void ASPlayerCharacter::StopFire(const FInputActionValue& InValue)
{
	// 연발 사격 타이머 클리어
	GetWorldTimerManager().ClearTimer(BetweenShotsTimer);
}
