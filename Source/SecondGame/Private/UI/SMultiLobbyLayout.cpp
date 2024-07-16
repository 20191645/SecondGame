// SMultiLobbyLayout.cpp

#include "UI/SMultiLobbyLayout.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/SUIPlayerController.h"

void USMultiLobbyLayout::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // ��ư Ŭ�� �̺�Ʈ�� ��� �Լ� ���ε�
    SubmitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);
}

void USMultiLobbyLayout::OnSubmitButtonClicked()
{
    // �÷��̾� �̸� �Է¹ޱ� (1-9����)
    FString PlayerName = EditPlayerName->GetText().ToString();
    if (PlayerName.Len() <= 0 || 10 <= PlayerName.Len())
    {
        return;
    }

    // �÷��̾� ���� ����("PlayerInfo.txt") ���� ��� ����
    const FString SavedDirectoryPath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));
    const FString SavedFileName(TEXT("PlayerInfo.txt"));
    FString AbsoluteFilePath = FPaths::Combine(*SavedDirectoryPath, *SavedFileName);
    FPaths::MakeStandardFilename(AbsoluteFilePath);

    // Json ������Ʈ ����
    // -- playerName: �÷��̾� �̸�
    TSharedRef<FJsonObject> PlayerInfoJsonObject = MakeShared<FJsonObject>();
    PlayerInfoJsonObject->SetStringField("playername", PlayerName);

    // Json ������Ʈ�� ����ȭ�Ͽ� �÷��̾� ���� ����("PlayerInfo.txt")�� ����
    FString PlayerInfoJsonString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriterArchive = TJsonWriterFactory<TCHAR>::Create(&PlayerInfoJsonString);
    if (FJsonSerializer::Serialize(PlayerInfoJsonObject, JsonWriterArchive) == true)
    {
        FFileHelper::SaveStringToFile(PlayerInfoJsonString, *AbsoluteFilePath);
    }

    // IP �ּҸ� ���� ���� ����
    ASUIPlayerController* PlayerController = GetOwningPlayer<ASUIPlayerController>();
    if (true == ::IsValid(PlayerController))
    {
        FText ServerIP = EditServerIP->GetText();
        PlayerController->JoinServer(ServerIP.ToString());
    }
}
