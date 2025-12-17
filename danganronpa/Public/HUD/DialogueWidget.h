#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DTReader.h"
#include "TimerManager.h"
#include "DialogueWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetAnimation;
class UImage;

/**
 *
 */
UCLASS()
class DANGANRONPA_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> MyDialogue;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> CameraDialogue;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> DrawerDialogue;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> PaperDialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>BroadcastAppear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation>BroadcastFade;



	// Bound TextBlock
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;
	int32 TextIndex = 0; // Ensure initialization
	int32 InvesIndex = -1;
	TArray<FDialogData*>Dialogues;
	TArray<FDialogData*>Dialogues_Inves;
	// TextBubble widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TextBubble;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Camera;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Paper;

	// Character portraits
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Naegi_normal;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Naegi_talking;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Naegi_confused;

	// Classroom background
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Classroom;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Broadcast;

protected:
	UFUNCTION()
	void OnBubbleClicked();

	UFUNCTION()
	void OnCameraClicked();

	UFUNCTION()
	void OnPaperClicked();


private:
	bool InMainDialogue = true;
	bool InvestigatePhase = false;
	bool PaperRead = false;
	bool CameraRead = false;
	bool bIsAnimating = false;

	//Refactored with Decorator Pattern
	struct IDlgStep;
	TUniquePtr<IDlgStep> RootStep; // Points to the first link in the decorator chain
	struct FDlgContext;
	TUniquePtr<FDlgContext> Ctx;   // Runtime context
	void BuildPipeline();          // Function for assembling the decorator chain
	

	void SetTextAndBubbleHidden();

	void SetTextAndBubbleVisible();
	// Get timer handle
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle2;
	void InitializePortraits();
	// Set the text box delay time according to the passed negative delay value
	void TextMoveOn(float delay);

	TObjectPtr<UImage> CurrentPortrait;

	void SetPortraitVisible();
	void SetPortraitHidden();
	void PlayAnimate();
	TObjectPtr<UDataTable> CurrentDialogue;
};


// Refactored with Decorator Pattern

// Lightweight context for passing Widget pointer between decorators
struct FDlgContext {
    class UDialogueWidget* Widget = nullptr;
};

// Interface for all dialogue "steps"
struct IDlgStep {
    virtual ~IDlgStep() {}
    virtual void Execute(FDlgContext& Ctx) = 0;
};

// Base class for all "wrappers" or "decorators"
struct FDlgDecorator : public IDlgStep {
    TUniquePtr<IDlgStep> Inner; // Holds the next step to be wrapped
    explicit FDlgDecorator(TUniquePtr<IDlgStep> In) : Inner(MoveTemp(In)) {}
    void Execute(FDlgContext& Ctx) override { if (Inner) Inner->Execute(Ctx); }
};

// Core step: only responsible for displaying the next text and advancing the index
struct FCoreStep : public IDlgStep {
    void Execute(FDlgContext& Ctx) override;
};

// Decorator #1: Block input if animation is playing
struct FInputGuardDeco : public FDlgDecorator {
    using FDlgDecorator::FDlgDecorator;
    void Execute(FDlgContext& Ctx) override;
};

// Decorator #2: Handle display/hide of the character portrait for the current line
struct FPortraitDeco : public FDlgDecorator {
    using FDlgDecorator::FDlgDecorator;
    void Execute(FDlgContext& Ctx) override;
};
