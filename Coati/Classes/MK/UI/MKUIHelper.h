#ifndef MK_UI_HELPER_H
#define MK_UI_HELPER_H

// Include cocos2d
#include "cocos2d.h"

// Include UI classes
#include "ui/UIButton.h"
#include "ui/UISlider.h"
#include "ui/UIScrollView.h"

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKAssertions.h"

// Namespaces
USING_NS_CC;

NS_MK_BEGIN

class MKUIHelper
{
private:
	// Constructor(s) & Destructor
	MKUIHelper() {}
	~MKUIHelper() {}

public:
	// Variable(s)
	static const mkString DEFAULT_FONT;
    static const mkF32 DEFAULT_BUTTON_ZOOM_SCALE;

	// Create Label
	static Label* CreateLabel(
		const mkString& _labelText,
		mkF32 _fontSize,
		const mkString& _fontName = DEFAULT_FONT)
	{
		// Find out the font type.
		MK_ASSERT((_fontName.length() > 4));
		mkString fileExtension;
		for (mkU32 i = _fontName.length() - 4; i < _fontName.length(); ++i)
		{
			fileExtension.push_back(_fontName[i]);
		}

		// Create Label
		Label* label = nullptr;
		if (fileExtension == ".ttf")
		{
			label = Label::createWithTTF(_labelText, _fontName, _fontSize);
		}
		else if (fileExtension == ".fon")
		{
			label = Label::createWithSystemFont(_labelText, _fontName, _fontSize);
		}
		MK_ASSERT((label != nullptr));

		return label;
	}

	// Create Button
	static ui::Button* CreateButton(
		const mkString& _labelText,
		mkF32 _fontSize,
		const mkString& _fontName,
		const mkString& _normalTexture,
		const mkString& _selectedTexture,
		const mkString& _disabledTexture,
		mkF32 _zoomScale = DEFAULT_BUTTON_ZOOM_SCALE,
		const std::function<void(Ref*)>& _callback = nullptr)
	{
		// Create Label
		auto label = CreateLabel(_labelText, _fontSize, _fontName);
		
		// Create Button
		auto button = ui::Button::create(_normalTexture, _selectedTexture, _disabledTexture);
		button->setTitleLabel(label);
		button->setZoomScale(_zoomScale);

		// Set click callback.
        if (_callback != nullptr)
        {
            button->addClickEventListener(_callback);
        }

		return button;
	}

	//Create Slider
	static ui::Slider* CreateSlider(
		const mkString& _barTexture,
		const mkString& _progressBarTexture,
		const mkString& _ballNormalTexture,
		const mkString& _ballSelectedTexture,
		const mkString& _ballDisabledTexture,
		mkF32 _zoomScale = 1.1f,
		const std::function<void(Ref*, ui::Slider::EventType)>& _callback = nullptr)
	{
		auto slider = ui::Slider::create();

		slider->loadBarTexture(_barTexture);
		slider->loadProgressBarTexture(_progressBarTexture);
		slider->loadSlidBallTextures(_ballNormalTexture, _ballSelectedTexture, _ballDisabledTexture);
		slider->setZoomScale(_zoomScale);

		// Add Slide Callback
        if (_callback != nullptr)
        {
            slider->addEventListener(_callback);
        }

		return slider;
	}

	// Create ScrollView
	static ui::ScrollView* CreateScrollView(
		const Size& _contentSize,
		const Size& _innerContainerSize,
		ui::ScrollView::Direction _direction,
		mkBool _bounceEnable = false)
	{
		auto scrollView = ui::ScrollView::create();
		scrollView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		scrollView->setContentSize(_contentSize);
		scrollView->setInnerContainerSize(_innerContainerSize);
		scrollView->setDirection(_direction);
		scrollView->setBounceEnabled(_bounceEnable);

		return scrollView;
	}
};

NS_MK_END

#endif // MK_UI_HELPER_H