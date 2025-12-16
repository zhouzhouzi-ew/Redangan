// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager.h"


bool ADialogueManager::GetNextDialogue(FString& OutCharacterName, FString& OutDialogueText, UTexture2D*& OutCharacterImage)
{
    if (!DialogueTable) return false;

    // 获取所有行的 ID
    static const FString ContextString(TEXT("Dialogue Context"));
    TArray<FName> RowNames = DialogueTable->GetRowNames();

    // 检查索引是否超出范围
    if (CurrentIndex >= RowNames.Num())
    {
        return false; // 对话结束
    }

    // 获取当前行数据
    FName CurrentRowName = RowNames[CurrentIndex];
    FDialogueRow* CurrentRow = DialogueTable->FindRow<FDialogueRow>(CurrentRowName, ContextString);

    if (CurrentRow)
    {
        OutCharacterName = CurrentRow->CharacterName;
        OutDialogueText = CurrentRow->DialogueText;
        OutCharacterImage = CurrentRow->CharacterImage;

        CurrentIndex++; // 推进到下一行
        return true;
    }

    return false;
}


// Sets default values
ADialogueManager::ADialogueManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADialogueManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

