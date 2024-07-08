// SUIPlayerController_Loading.cpp

#include "Controller/SUIPlayerController_Loading.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

void ASUIPlayerController_Loading::BeginPlay()
{
	Super::BeginPlay();

	// 타이틀 레벨로부터 전달받은 'NextLevel'을 파싱해서 이동할 레벨 이름을 얻어낸다
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == true)
	{
		FString NextLevelString = UGameplayStatics::ParseOption(GameMode->OptionsString, FString(TEXT("NextLevel")));
		UGameplayStatics::OpenLevel(GameMode, *NextLevelString, false);
	}
}
