// SUIPlayerController.cpp

#include "Controller/SUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (true == ::IsValid(UIWidgetClass))
	{
		// CreateWidget()이 호출될 때 'UIWidgetInstance'의 NativeOnInitialize() 함수가 호출됨
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if (true == ::IsValid(UIWidgetInstance))
		{
			// AddToViewport()가 호출 될 때 'UIWidgetInstance'의 NativeConstruct() 함수가 호출됨
			UIWidgetInstance->AddToViewport();

			// UI 모드 적용
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			// 마우스 커서 보이기
			bShowMouseCursor = true;
		}
	}
}

void ASUIPlayerController::JoinServer(const FString& InIPAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingLevel"), true, FString::Printf(TEXT("NextLevel=%s"), *InIPAddress));
}
