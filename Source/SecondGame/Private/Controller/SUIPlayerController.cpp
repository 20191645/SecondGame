// SUIPlayerController.cpp

#include "Controller/SUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (true == ::IsValid(UIWidgetClass))
	{
		// CreateWidget()�� ȣ��� �� 'UIWidgetInstance'�� NativeOnInitialize() �Լ��� ȣ���
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if (true == ::IsValid(UIWidgetInstance))
		{
			// AddToViewport()�� ȣ�� �� �� 'UIWidgetInstance'�� NativeConstruct() �Լ��� ȣ���
			UIWidgetInstance->AddToViewport();

			// UI ��� ����
			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			// ���콺 Ŀ�� ���̱�
			bShowMouseCursor = true;
		}
	}
}

void ASUIPlayerController::JoinServer(const FString& InIPAddress)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LoadingLevel"), true, FString::Printf(TEXT("NextLevel=%s"), *InIPAddress));
}
