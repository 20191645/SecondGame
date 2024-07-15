// SMultiLobbyLayout.cpp

#include "UI/SMultiLobbyLayout.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/SUIPlayerController.h"

void USMultiLobbyLayout::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // 버튼 클릭 이벤트와 멤버 함수 바인드
    SubmitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);
}

void USMultiLobbyLayout::OnSubmitButtonClicked()
{
    // 플레이어 이름 입력받기 (1-9글자)
    FString PlayerName = EditPlayerName->GetText().ToString();
    if (PlayerName.Len() <= 0 || 10 <= PlayerName.Len())
    {
        return;
    }

    // 플레이어 정보 파일("PlayerInfo.txt") 저장 경로 설정
    const FString SavedDirectoryPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
    const FString SavedFileName(TEXT("PlayerInfo.txt"));
    FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
    FPaths::MakeStandardFilename(AbsoluteFilePath);

    // Json 오브젝트 생성
    // -- playerName: 플레이어 이름
    TSharedRef<FJsonObject> PlayerInfoJsonObject = MakeShared<FJsonObject>();
    PlayerInfoJsonObject->SetStringField("playername", PlayerName);

    // Json 오브젝트를 직렬화하여 플레이어 정보 파일("PlayerInfo.txt")에 저장
    FString PlayerInfoJsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriterArchive = TJsonWriterFactory<TCHAR>::Create(&PlayerInfoJsonString);
    if (FJsonSerializer::Serialize(PlayerInfoJsonObject, JsonWriterArchive) == true)
    {
        FFileHelper::SaveStringToFile(PlayerInfoJsonString, *AbsoluteFilePath);
    }

    // IP 주소를 통해 서버 접속
    ASUIPlayerController* PlayerController = GetOwningPlayer<ASUIPlayerController>();
    if (true == ::IsValid(PlayerController))
    {
        FText ServerIP = EditServerIP->GetText();
        PlayerController->JoinServer(ServerIP.ToString());
    }
}
