#include "searchhightlight.h"

#include <QTextCharFormat>

SearchHighLight::SearchHighLight(QTextDocument* parent) : BaseClass(parent){
    format_.setBackground(Qt::green);
}

void SearchHighLight::highlightBlock(const QString& text){
    QRegularExpressionMatchIterator matchIterator = pattern_.globalMatch(text);
    while (matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), format_);
    }
}

void SearchHighLight::searchText(const QString& text){
    pattern_ = QRegularExpression(text);
    rehighlight();
}
