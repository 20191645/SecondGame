// SPlayerController.cpp

#include "Controller/SPlayerController.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// SetInputMode(): 플레이 버튼 누르면 바로 포커싱 되게끔 함
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if (IsValid(CrosshairUIClass) == true)
	{
		UUserWidget* CrosshairUIInstance = CreateWidget<UUserWidget>(this, CrosshairUIClass);
		if (IsValid(CrosshairUIInstance) == true)
		{
			// 조준점 위젯 화면에 추가
			CrosshairUIInstance->AddToViewport(1);
			CrosshairUIInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
