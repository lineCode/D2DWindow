﻿/*
The MIT License (MIT)
Copyright (c) 2015 admin@sugarontop.net
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "stdafx.h"
#include "D2DWindowMessage.h"
#include "D2DWindowControl_easy.h"
#include "D2DCommon.h"
#include "D2DWindowImageButtons.h"


/*
	a: one button only in many buttons. exclusive is true.
		dispable
		on
		off
		click only

	b : some buttons in many buttons.
		disable
		on
		off
*/




namespace V4 {
D2DImageButtons::D2DImageButtons()
{

}

void D2DImageButtons::CreateWindow(D2DWindow* parent, D2DControls* pacontrol, const FRectFBoxModel& rct, int stat, LPCWSTR name, int id )
{
	FRectFBoxModel rc = rct;
	bresize_width = ( rc.Size().width <= 0 );
	if (bresize_width )
	{
		rc.left = 0;
		rc.right = 10000;
	}

	D2DControl::CreateWindow(parent, pacontrol, rc, stat, name, id);


	btn_cnt_ = 0;
	img_ = nullptr;
	float_idx_ = -1;

}

LRESULT D2DImageButtons::WndProc(D2DWindow* d, UINT message, WPARAM wParam, LPARAM lParam)
{
	int ret = 0;

	if ( !VISIBLE(stat_))
		return ret;

	switch( message )
	{
		case WM_PAINT:
		{
			OnPaintType2(d);

		}
		break;
		case WM_LBUTTONDOWN:
		{
			FRectF rcb = rc_.GetContentRect();
			FPointF pt = mat_.DPtoLP(lParam);
			if (rcb.PtInRect(pt))
			{
				pt.x -= rcb.left;
				pt.y -= rcb.top;

				int other_all_off = 0;

				for (int i = 0; i < btn_cnt_; i++)
				{
					DWORD& md = btn_mode_.get()[i];

					if ( rcs_.get()[i].PtInRect(pt) )
					{
						d->SetCapture(this);

						ret = 1;

						if (IS_ON(md))
							md &= ~STAT_ON;
						else
						{
							md |= STAT_ON;
						
							if ( md & MODE_ONOFF_OTHERALLOFF )
								other_all_off++;
						}

						last_idx_ = i;
					}
					else
					{
						md &= ~STAT_ON;
					}	
				}

				
				
				if ( other_all_off )
				{
					for (int i = 0; i < btn_cnt_; i++)
					{
						if ( i != last_idx_ )
						{
							DWORD& md = btn_mode_.get()[i];
							md &= ~STAT_ON;
						}
					}
				}
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			int old = float_idx_;
			float_idx_ = -1;
			FRectF rcb = rc_.GetContentRect();
			FPointF pt = mat_.DPtoLP(lParam);
			if (rcb.PtInRect(pt))
			{
				pt.x -= rcb.left;
				pt.y -= rcb.top;
				float_idx_ = -1;
				for (int i = 0; i < btn_cnt_; i++)
				{
					if (rcs_.get()[i].PtInRect(pt))
					{

						DWORD& md = btn_mode_.get()[i];
						if ( md & MODE_ENABLE )
							float_idx_ = i;

						ret = 1;
						break;
					}
				}
			}

			if (old != float_idx_ )
				d->redraw_ = 1;

		}
		break;
		case WM_LBUTTONUP:
		{
			if (d->GetCapture() == this)
			{
				d->ReleaseCapture();
				ret = 1;


				DWORD& md = btn_mode_.get()[last_idx_];
				if (IS_CLICKONLY(md))
					md &= ~STAT_ON;

				

				if (!IS_DISABLE(md) && onclick_ )
				{
					FRectF rcb = rc_.GetContentRect();
					FPointF pt = mat_.DPtoLP(lParam);
					if (rcb.PtInRect(pt))
					{
						onclick_( this, last_idx_);
					}
				}
			}

		}
		break;
		case WM_SIZE:
		{
			auto sz = parent_control_->GetRect().Size();

			if (bresize_width )
			{
				rc_.left = 0;
				rc_.right = sz.width;

			}

		}
		break;
	};

	return ret;
}
void D2DImageButtons::OnPaintType2(D2DWindow* d)
{
	auto& cxt = d->cxt_;
	D2DMatrix mat(d->cxt_);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);
	FillRectangle(cxt, FRectF(-2, -2, 2, 2), (ID2D1Brush*) cxt.red);
	FillRectangle(cxt, rc_.GetContentRectZero(), (ID2D1Brush*)back_color_.br);

	if (bmp)
	{
		FRectF rc(FPointF(0, 0), bmp->GetSize());
		cxt.cxt->DrawBitmap(bmp, rc);
	}

	if (float_idx_ > -1)
	{
		FRectF rc = rcs_.get()[float_idx_];
		FillRectangle(cxt, rc, (ID2D1Brush*) floatbr_.br);
	}

	for (int i = 0; i < btn_cnt_; i++)
	{
		DWORD md = btn_mode_.get()[i];
		FRectF rc = rcs_.get()[i];
		if (IS_DISABLE(md))
		{
			FillRectangle(cxt, rc, (ID2D1Brush*) disablebr_.br);
		}
		else if (IS_ON(md))
		{
			FillRectangle(cxt, rc, (ID2D1Brush*) onbr_.br);
		}
		else
		{
			if (i != float_idx_)
				FillRectangle(cxt, rc, (ID2D1Brush*) offbr_.br);
		}
	}

	/*if ( bmp )
	{
	FRectF rc( FPointF(0,0), bmp->GetSize());
	cxt.cxt->DrawBitmap( bmp, rc );
	}*/

	mat.PopTransform();


}
void D2DImageButtons::OnPaintType1(D2DWindow* d )
{
	auto& cxt = d->cxt_;
	D2DMatrix mat(d->cxt_);
	mat_ = mat.PushTransform();
	mat.Offset(rc_);


	FillRectangle(cxt, rc_.GetContentRectZero(), (ID2D1Brush*) offbr_.br);

	if (float_idx_ > -1)
	{
		FRectF rc = rcs_.get()[float_idx_];
		FillRectangle(cxt, rc, (ID2D1Brush*) floatbr_.br);
	}

	for (int i = 0; i < btn_cnt_; i++)
	{
		DWORD md = btn_mode_.get()[i];
		FRectF rc = rcs_.get()[i];
		if (IS_DISABLE(md))
		{
			FillRectangle(cxt, rc, (ID2D1Brush*) disablebr_.br);
		}
		else if (IS_ON(md))
		{
			FillRectangle(cxt, rc, (ID2D1Brush*) onbr_.br);
		}
		else
		{
			if (i != float_idx_)
				FillRectangle(cxt, rc, (ID2D1Brush*) offbr_.br);
		}
	}

	if (bmp)
	{
		FRectF rc(FPointF(0, 0), bmp->GetSize());
		cxt.cxt->DrawBitmap(bmp, rc);
	}

	mat.PopTransform();
}

void D2DImageButtons::SetButtons( const D2D1_RECT_F* rcs, DWORD* mode, int cnt)
{
	std::shared_ptr<DWORD> md = std::shared_ptr<DWORD>(new DWORD[cnt], std::default_delete<DWORD []>());
	std::shared_ptr<FRectF> x = std::shared_ptr<FRectF>( new FRectF[cnt], std::default_delete<FRectF[]>() );
	btn_cnt_ = cnt;
	for( int i = 0; i < btn_cnt_; i++ )
	{
		x.get()[i] = rcs[i];
		md.get()[i] = mode[i];
	}

	rcs_ = x;
	btn_mode_ = md;
}

void D2DImageButtons::SetButtonImage(void* img, int length )
{	
	img_bin_ = Binary( (const BYTE*)img, (DWORD)length );
}



void D2DImageButtons::SetBackColor(D2D1_COLOR_F offclr, D2D1_COLOR_F onclr, D2D1_COLOR_F floatingclr, D2D1_COLOR_F disableclr)
{
	_ASSERT(img_bin_.get());

	back_color_.color = offclr;
	offbr_.color = offclr;
	onbr_.color = onclr;
	floatbr_.color = floatingclr;
	disablebr_.color = disableclr;

	OnResutructRnderTarget(true);
}
void D2DImageButtons::SetParameters(const std::map<std::wstring, VARIANT>& prms)
{	
	ParameterColor(parent_, back_color_, prms, L"backcolor");
	ParameterColor(parent_, onbr_, prms, L"on_color");
	ParameterColor(parent_, offbr_, prms, L"off_color");
	ParameterColor(parent_, floatbr_, prms, L"floating_color");
	ParameterColor(parent_, disablebr_, prms, L"disable_color");
}

void D2DImageButtons::OnResutructRnderTarget(bool bCreate)
{
	if (bCreate)
	{
		back_color_.br = parent_->GetSolidColor(back_color_.color);
		offbr_.br = parent_->GetSolidColor(offbr_.color);
		onbr_.br = parent_->GetSolidColor(onbr_.color);
		floatbr_.br = parent_->GetSolidColor(floatbr_.color);
		disablebr_.br = parent_->GetSolidColor(disablebr_.color);

		CreateD2D1Bitmap( parent_->cxt_, &bmp, img_bin_); 
	}
	else
	{
		offbr_.br = nullptr;
		onbr_.br = nullptr;
		floatbr_.br = nullptr;
		disablebr_.br = nullptr;

		bmp.Release();
	}
}



};
