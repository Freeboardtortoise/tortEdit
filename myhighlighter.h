#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QColor>
#include <QVector>
#include <QJsonObject>

class MyHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit MyHighlighter(QTextDocument *parent, const QString &themeFile = "");

    void loadThemeFromJson(const QString &filePath);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightRule> rules;
    QTextCharFormat multiLineCommentFormat;

    QRegularExpression commentStart;
    QRegularExpression commentEnd;

    void buildRules();
    QTextCharFormat createFormat(const QColor &color, bool bold = false, bool italic = false);
};

#endif // MYHIGHLIGHTER_H
