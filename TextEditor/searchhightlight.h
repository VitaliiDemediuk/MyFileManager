#ifndef SEARCHHIGHTLIGHT_H
#define SEARCHHIGHTLIGHT_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SearchHighLight : public QSyntaxHighlighter
{
    Q_OBJECT
    using BaseClass = QSyntaxHighlighter;
public:
    explicit SearchHighLight(QTextDocument* parent = nullptr);

    void searchText(const QString& text);

protected:
    virtual void highlightBlock(const QString &text) override;

private:
    QRegularExpression pattern_;
    QTextCharFormat format_;
};

#endif // SEARCHHIGHTLIGHT_H
