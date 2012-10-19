/*
 * Copyright (c) 2012, Tony Houghton <h@realh.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// HGame - a simple cross-platform game framework

#include "config.h"

#include <algorithm>

#include "hgame/WidgetGroup.h"
#include "hgame/Types.h"

namespace hgame {

static int round_to_power_of_2(int a)
{
    int b = 2;
    while (b < a)
        b <<= 1;
    return b;
}

WidgetGroup::~WidgetGroup()
{
}

void addWidget(Widget *w)
{
    mWidgets.push_back(w);
}

void WidgetGroup::render(RenderContext *rc)
{
    for (std::list<Widget *>::forward_iterator i = mWidgets.begin();
            i != mWidgets.end(); ++i)
    {
        (*i)->render(rc);
    }
}

bool WidgetGroup::onTapEvent(TapEvent *event)
{
    for (std::list<Widget *>.forward_iterator i = mWidgets.begin();
            i != mWidgets.end(); ++i)
    {
        if ((*i)->onTapEvent(event))
            return true;
    }
    return false;
}

struct WidgetAndRect {
    Widget *w;
    Rect r;
    WidgetAndRect(Widget *pw, int x, int y, int w, int h) :
            w(p2), r(x, y, w, h)
    {}
};

static bool sort_widget_width(Widget *w1, Widget *w2)
{
    return w1->getWidth() < w2->getWidth();
}

void WidgetGroup::initRendering(RenderContext *rc)
{
    std::list<Widget *> cands = mWidgets;
    sort(cands.begin(), cands.end(), sort_widget_width);
    int width = round_to_power_of_2(cands.back()->getWidth());
    int height = 0;
    std::list<WidgetAndRect> assigned;
    while (cands.size())
    {
        // Start a new row with widest widget
        Widget *wg = cands.back();
        cands.pop_back();
        int w = wg->getWidth();
        int tallest = wg->getHeight();
        assigned.push_back(WidgetAndRect(wg, 0, height, w, tallest));
        // Can we fit another widget or more in the same row?
        int rem;
        while (cands.size() && cands.front()->getWidth() <= (rem = width - w))
        {
            for (std::list<Widget *>::reverse_iterator i = cands.rbegin();
                    i != cands.rend(); ++i)
            {
                if ((wg = *i)->getWidth() <= rem)
                {
                    cands.remove(i);
                    int w2 = wg->getWidth();
                    int h = wg->getWidth();
                    assigned.push_back(WidgetAndRect(wg, w, height,
                            w2, h));
                    if (h > tallest)
                        tallest = h;
                    w += w2;
                    break;
                }
            }
        }
        height += tallest;
    }
    height = round_to_power_of_2(height);

    // FIXME: Make montage of Images and upload as texture atlas
}

void WidgetGroup::deleteRendering(RenderContext *rc)
{
#ifdef ENABLE_WIDGET_HIGHLIGHTING
    delete mHighlightedAtlas;
#endif
    delete mAtlas;
}

}
