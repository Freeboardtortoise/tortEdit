#include "myhighlighter.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

MyHighlighter::MyHighlighter(QTextDocument *parent, const QString &themeFile)
    : QSyntaxHighlighter(parent)
{
    commentStart = QRegularExpression("/\\*");
    commentEnd = QRegularExpression("\\*/");

    // Load theme or fall back to defaults
    if (!themeFile.isEmpty())
        loadThemeFromJson(themeFile);
    else {
        // Set default hardcoded colors
        QTextCharFormat keywordFmt = createFormat(Qt::blue, true);
        QTextCharFormat stringFmt = createFormat(Qt::red);
        QTextCharFormat commentFmt = createFormat(Qt::darkGreen);
        multiLineCommentFormat = createFormat(Qt::gray, false, true);

        rules = {
            { QRegularExpression("\\bint\\b|\\bfloat\\b|\\breturn\\b|\\bif\\b|\\belse\\b|\\bwhile\\b|\\bvoid\\b|\\bfor\\b|\\bclass\\b|\\bstatic\\b"), keywordFmt },
            { QRegularExpression("\".*?\""), stringFmt },
            { QRegularExpression("//[^\n]*"), commentFmt }
        };
    }
}

void MyHighlighter::loadThemeFromJson(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to load theme JSON:" << filePath;
        return;
    }
    qDebug() << "Loaded" << rules.size() << "highlighting rules.";


    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in theme file.";
        return;
    }

    QJsonObject obj = doc.object();

    QTextCharFormat keywordFmt = createFormat(QColor(obj["keywordColor"].toString()), true);
    QTextCharFormat stringFmt = createFormat(QColor(obj["stringColor"].toString()));
    QTextCharFormat commentFmt = createFormat(QColor(obj["commentColor"].toString()));
    multiLineCommentFormat = createFormat(QColor(obj["multilineCommentColor"].toString()), false, true);

    rules = {
        { QRegularExpression("\\bint\\b|\\bfloat\\b|\\breturn\\b|\\bif\\b|\\belse\\b"), keywordFmt },
        { QRegularExpression("\".*?\""), stringFmt },
        { QRegularExpression("//[^\n]*"), commentFmt }
    };

    rehighlight();  // apply changes immediately
}

QTextCharFormat MyHighlighter::createFormat(const QColor &color, bool bold, bool italic)
{
    QTextCharFormat format;
    format.setForeground(color);
    if (bold) format.setFontWeight(QFont::Bold);
    if (italic) format.setFontItalic(true);
    return format;
}

void MyHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightRule &rule : rules) {
        auto matches = rule.pattern.globalMatch(text);
        while (matches.hasNext()) {
            auto match = matches.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
    int start = previousBlockState() != 1 ? text.indexOf(commentStart) : 0;

    while (start >= 0) {
        int end = text.indexOf(commentEnd, start);
        int length;
        if (end == -1) {
            setCurrentBlockState(1);
            length = text.length() - start;
        } else {
            length = end - start + 2;
        }
        setFormat(start, length, multiLineCommentFormat);
        start = text.indexOf(commentStart, start + length);
    }
}
