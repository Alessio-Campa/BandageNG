//Copyright 2017 Ryan Wick

//This file is part of Bandage

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.


#include "settings.h"
#include "graph/nodecolorer.h"
#include <QDir>
#include <QSettings>
#include <QVariantList>
#include <QVariantMap>
#include <algorithm>

Settings::Settings()
{
    doubleMode = false;

    nodeLengthMode = AUTO_NODE_LENGTH;
    autoNodeLengthPerMegabase = 1000.0;
    manualNodeLengthPerMegabase = FloatSetting(1000.0, 0, 1000000.0);
    meanNodeLength = 40.0;
    minTotalGraphLength = 500.0;
    graphLayoutQuality = IntSetting(2, 0, 4);
    linearLayout = false;
    minimumNodeLength = FloatSetting(5.0, 1.0, 100.0);
    edgeLength = FloatSetting(5.0, 0.1, 100.0);
    doubleModeNodeSeparation = FloatSetting(2.0, 0.0, 100.0);
    nodeSegmentLength = FloatSetting(20.0, 1.0, 1000.0);
    componentSeparation = FloatSetting(50.0, 0, 1000.0);

    averageNodeWidth = FloatSetting(5.0, 0.5, 1000.0);
    depthEffectOnWidth = FloatSetting(0.5, 0.0, 1.0);
    depthPower = FloatSetting(0.5, 0.0, 1.0);

    jumpsAsLinks = false;
    edgeWidth = FloatSetting(1.5, 0.1, 100);
    linkWidth = FloatSetting(0.5, 0.1, 100);
    outlineThickness = FloatSetting(0.0, 0.0, 100.0);
    selectionThickness = 1.0;
    arrowheadsInSingleMode = false;
    textOutlineThickness = FloatSetting(1.5, 0.0, 10.0);

    rainbowPartsPerQuery = 100;

    graphScope = WHOLE_GRAPH;
    nodeDistance = IntSetting(0, 0, 100);
    startingNodesExactMatch = true;
    startingNodes = "";
    startingPaths = "";
    startingWalks = "";
    queryFilename = "";
    unnamedQueryDefaultName = "unnamed";

    minZoom = 0.001;
    minZoomOnGraphDraw = 0.001;
    maxZoom = 100.0;
    maxAutomaticZoom = 10.0;
    zoomFactor = 1.0005;
    textZoomScaleFactor = 0.7;

    dragStrength = 100.0;

    displayNodeCustomLabels = false;
    displayNodeNames = false;
    displayNodeLengths = false;
    displayNodeDepth = false;
    displayNodeCsvData = false;
    displayNodeCsvDataCol = 0;
    labelFont = QFont();
    textOutline = false;
    antialiasing = true;
    positionTextNodeCentre = false;

    nodeDragging = NEARBY_PIECES;

    initializeColorer(RANDOM_COLOURS);
    uniformPositiveNodeColour = QColor(178, 34, 34);
    uniformNegativeNodeColour = QColor(128, 0, 0);
    uniformNodeSpecialColour = QColor(0, 128, 0);

    randomColourPositiveOpacity = IntSetting(255, 0, 255);
    randomColourNegativeOpacity = IntSetting(255, 0, 255);
    randomColourPositiveSaturation = IntSetting(127, 0, 255);
    randomColourNegativeSaturation = IntSetting(127, 0, 255);
    randomColourPositiveLightness = IntSetting(150, 0, 255);
    randomColourNegativeLightness = IntSetting(90, 0, 255);

    edgeColour = QColor(0, 0, 0, 180);
    outlineColour = QColor(0, 0, 0);
    selectionColour = QColor(0, 0, 255);
    textColour = QColor(0, 0, 0);
    textOutlineColour = QColor(255, 255, 255);

    contiguitySearchSteps = IntSetting(15, 1, 50);
    contiguousStrandSpecificColour = QColor(0, 155, 0);
    contiguousEitherStrandColour = QColor(0, 155, 0);
    maybeContiguousColour = QColor(190, 240, 190);
    notContiguousColour = QColor(220, 220, 220);
    contiguityStartingColour = QColor(0, 255, 0);

    grayColor = QColor(220, 220, 220);

    autoDepthValue = true;
    lowDepthValue = FloatSetting(5.0, 0.0, 1000000.0);
    highDepthValue = FloatSetting(50.0, 0.0, 1000000.0);
    colorMap = ColorMap::Viridis;

    pathHighlightShadingColour = QColor(0, 0, 0, 60);
    pathHighlightOutlineColour = QColor(0, 0, 0);

    defaultCustomNodeColour = QColor(190, 190, 190);

    tagColourPalette = defaultTagColourPalette();
    conditionalColour = QColor(220, 220, 220);

    minAutoFindEdgeOverlap = 10;
    maxAutoFindEdgeOverlap = 200;

    maxHitsForQueryPath = IntSetting(100, 0, 1000000);
    maxQueryPathNodes = IntSetting(6, 1, 50);
    minQueryCoveredByPath = FloatSetting(0.9, 0.15, 1.0);
    minQueryCoveredByHits = FloatSetting(0.9, 0.15, 1.0, true);
    minMeanHitIdentity = FloatSetting(0.5, 0.0, 1.0, true);
    maxEValueProduct = SciNotSetting(SciNot(1.0, -10), SciNot(1.0, -999), SciNot(9.9, 1), true);
    minLengthPercentage = FloatSetting(0.95, 0.0, 10000.0, true);
    maxLengthPercentage = FloatSetting(1.05, 0.0, 10000.0, true);
    minLengthBaseDiscrepancy = IntSetting(-100, -1000000, 1000000, false);
    maxLengthBaseDiscrepancy = IntSetting(100, -1000000, 1000000, false);

    searchParameters = "";

    alignmentLengthFilter = IntSetting(100, 1, 1000000, false);
    queryCoverageFilter = FloatSetting(50.0, 0.0, 100.0, false);
    identityFilter = FloatSetting(90.0, 0.0, 100.0, false);
    evalueFilter = SciNotSetting(SciNot(1.0, -10), SciNot(1.0, -999), SciNot(9.9, 1), false);
    bitScoreFilter = FloatSetting(1000.0, 0.0, 1000000.0, false);
    blastAnnotationGroupName = "Blast Hits";
    bedAnnotationGroupName = "BED";

    minDepthRange = FloatSetting(10.0, 0.0, 1000000.0);
    maxDepthRange = FloatSetting(100.0, 0.0, 1000000.0);

    annotationsSettings = {};
}


QVector<QColor> Settings::defaultTagColourPalette()
{
    return {
        QColor("#1f77b4"), QColor("#ff7f0e"), QColor("#2ca02c"), QColor("#d62728"),
        QColor("#9467bd"), QColor("#8c564b"), QColor("#e377c2"), QColor("#7f7f7f"),
        QColor("#bcbd22"), QColor("#17becf"), QColor("#aec7e8"), QColor("#ffbb78"),
        QColor("#98df8a"), QColor("#ff9896"), QColor("#c5b0d5"), QColor("#c49c94"),
        QColor("#f7b6d2"), QColor("#c7c7c7"), QColor("#dbdb8d"), QColor("#9edae5")
    };
}


QString Settings::tagValueColourKey(const QString &tagName, const QString &value)
{
    return tagName + QChar(0x1f) + value;
}


void Settings::initializeColorer(NodeColorScheme scheme) {
    nodeColorer = INodeColorer::create(scheme);
}


namespace {

//All persisted options live in their own settings group so they cannot clash
//with other values (e.g. the recent file list).
const QString optionsPrefix = QStringLiteral("options/");

QString optionKey(const char *name)
{
    return optionsPrefix + QLatin1String(name);
}

QSettings persistentSettings()
{
    return QSettings("BandageNG", "BandageNG");
}

QVariant storedValue(QSettings &settings, const char *name)
{
    return settings.value(optionKey(name));
}

template <typename T>
void loadValue(QSettings &settings, const char *name, T &target)
{
    const QVariant value = storedValue(settings, name);
    if (value.isValid())
        target = value.value<T>();
}

template <typename T>
void saveValue(QSettings &settings, const char *name, const T &value)
{
    settings.setValue(optionKey(name), QVariant::fromValue(value));
}

void loadFloat(QSettings &settings, const char *name, FloatSetting &setting)
{
    const QVariant value = storedValue(settings, name);
    if (value.isValid())
        setting = std::clamp(value.toDouble(), setting.min, setting.max);
}

void saveFloat(QSettings &settings, const char *name, const FloatSetting &setting)
{
    settings.setValue(optionKey(name), setting.val);
}

void loadInt(QSettings &settings, const char *name, IntSetting &setting)
{
    const QVariant value = storedValue(settings, name);
    if (value.isValid())
        setting = std::clamp(value.toInt(), setting.min, setting.max);
}

void saveInt(QSettings &settings, const char *name, const IntSetting &setting)
{
    settings.setValue(optionKey(name), setting.val);
}

void loadToggle(QSettings &settings, const char *name, bool &on)
{
    const QVariant value = settings.value(optionKey(name) + QStringLiteral("On"));
    if (value.isValid())
        on = value.toBool();
}

void saveToggle(QSettings &settings, const char *name, const bool &on)
{
    settings.setValue(optionKey(name) + QStringLiteral("On"), on);
}

void loadSciNot(QSettings &settings, const char *name, SciNotSetting &setting)
{
    const QVariant coefficient = settings.value(optionKey(name) + QStringLiteral("Coefficient"));
    const QVariant exponent = settings.value(optionKey(name) + QStringLiteral("Exponent"));
    if (!coefficient.isValid() || !exponent.isValid())
        return;

    const SciNot value(coefficient.toDouble(), exponent.toInt());
    if (setting.inRange(value))
        setting = value;
}

void saveSciNot(QSettings &settings, const char *name, const SciNotSetting &setting)
{
    settings.setValue(optionKey(name) + QStringLiteral("Coefficient"),
                      setting.val.getCoefficient());
    settings.setValue(optionKey(name) + QStringLiteral("Exponent"),
                      setting.val.getExponent());
}

template <typename EnumType>
void loadEnum(QSettings &settings, const char *name, EnumType &target, int minimum, int maximum)
{
    const QVariant value = storedValue(settings, name);
    if (!value.isValid())
        return;

    const int intValue = value.toInt();
    if (intValue >= minimum && intValue <= maximum)
        target = EnumType(intValue);
}

void saveEnum(QSettings &settings, const char *name, int value)
{
    settings.setValue(optionKey(name), value);
}

QVariantList coloursToVariantList(const QVector<QColor> &colours)
{
    QVariantList list;
    list.reserve(colours.size());
    for (const QColor &colour : colours)
        list.append(colour);
    return list;
}

QVector<QColor> coloursFromVariantList(const QVariantList &list)
{
    QVector<QColor> colours;
    colours.reserve(list.size());
    for (const QVariant &value : list) {
        const QColor colour = value.value<QColor>();
        if (colour.isValid())
            colours.append(colour);
    }
    return colours;
}

}

void Settings::loadPersisted()
{
    QSettings settings = persistentSettings();

    loadValue(settings, "doubleMode", doubleMode);
    loadEnum(settings, "nodeLengthMode", nodeLengthMode, AUTO_NODE_LENGTH, MANUAL_NODE_LENGTH);
    loadFloat(settings, "manualNodeLengthPerMegabase", manualNodeLengthPerMegabase);
    loadInt(settings, "graphLayoutQuality", graphLayoutQuality);
    loadValue(settings, "linearLayout", linearLayout);
    loadFloat(settings, "minimumNodeLength", minimumNodeLength);
    loadFloat(settings, "edgeLength", edgeLength);
    loadFloat(settings, "doubleModeNodeSeparation", doubleModeNodeSeparation);
    loadFloat(settings, "nodeSegmentLength", nodeSegmentLength);
    loadFloat(settings, "componentSeparation", componentSeparation);

    loadFloat(settings, "depthEffectOnWidth", depthEffectOnWidth);
    loadFloat(settings, "depthPower", depthPower);
    loadFloat(settings, "edgeWidth", edgeWidth);
    loadFloat(settings, "linkWidth", linkWidth);
    loadFloat(settings, "outlineThickness", outlineThickness);
    loadValue(settings, "arrowheadsInSingleMode", arrowheadsInSingleMode);
    loadFloat(settings, "textOutlineThickness", textOutlineThickness);
    loadValue(settings, "rainbowPartsPerQuery", rainbowPartsPerQuery);

    loadValue(settings, "edgeColour", edgeColour);
    loadValue(settings, "outlineColour", outlineColour);
    loadValue(settings, "selectionColour", selectionColour);
    loadValue(settings, "textColour", textColour);
    loadValue(settings, "textOutlineColour", textOutlineColour);
    loadValue(settings, "uniformPositiveNodeColour", uniformPositiveNodeColour);
    loadValue(settings, "uniformNegativeNodeColour", uniformNegativeNodeColour);
    loadValue(settings, "uniformNodeSpecialColour", uniformNodeSpecialColour);

    loadInt(settings, "randomColourPositiveOpacity", randomColourPositiveOpacity);
    loadInt(settings, "randomColourNegativeOpacity", randomColourNegativeOpacity);
    loadInt(settings, "randomColourPositiveSaturation", randomColourPositiveSaturation);
    loadInt(settings, "randomColourNegativeSaturation", randomColourNegativeSaturation);
    loadInt(settings, "randomColourPositiveLightness", randomColourPositiveLightness);
    loadInt(settings, "randomColourNegativeLightness", randomColourNegativeLightness);

    loadInt(settings, "contiguitySearchSteps", contiguitySearchSteps);
    loadValue(settings, "contiguousStrandSpecificColour", contiguousStrandSpecificColour);
    loadValue(settings, "contiguousEitherStrandColour", contiguousEitherStrandColour);
    loadValue(settings, "maybeContiguousColour", maybeContiguousColour);
    loadValue(settings, "notContiguousColour", notContiguousColour);
    loadValue(settings, "contiguityStartingColour", contiguityStartingColour);

    loadValue(settings, "grayColor", grayColor);
    loadValue(settings, "autoDepthValue", autoDepthValue);
    loadFloat(settings, "lowDepthValue", lowDepthValue);
    loadFloat(settings, "highDepthValue", highDepthValue);
    loadEnum(settings, "colorMap", colorMap, int(Viridis), int(Cubehelix));

    loadValue(settings, "pathHighlightShadingColour", pathHighlightShadingColour);
    loadValue(settings, "pathHighlightOutlineColour", pathHighlightOutlineColour);
    loadValue(settings, "defaultCustomNodeColour", defaultCustomNodeColour);

    loadValue(settings, "displayNodeNames", displayNodeNames);
    loadValue(settings, "displayNodeLengths", displayNodeLengths);
    loadValue(settings, "displayNodeDepth", displayNodeDepth);
    loadValue(settings, "labelFont", labelFont);
    loadValue(settings, "textOutline", textOutline);
    loadValue(settings, "antialiasing", antialiasing);
    loadValue(settings, "positionTextNodeCentre", positionTextNodeCentre);

    loadEnum(settings, "graphScope", graphScope, int(WHOLE_GRAPH), int(AROUND_COMPONENT));
    //Scopes which depend on transient data (queries, paths, walks) fall back to
    //the whole graph, since that data does not exist yet in a fresh session.
    if (graphScope == AROUND_PATHS || graphScope == AROUND_WALKS ||
        graphScope == AROUND_QUERY_HITS || graphScope == AROUND_COMPONENT)
        graphScope = WHOLE_GRAPH;
    loadInt(settings, "nodeDistance", nodeDistance);
    loadValue(settings, "startingNodesExactMatch", startingNodesExactMatch);
    loadValue(settings, "startingNodes", startingNodes);
    loadFloat(settings, "minDepthRange", minDepthRange);
    loadFloat(settings, "maxDepthRange", maxDepthRange);

    loadValue(settings, "searchParameters", searchParameters);

    loadInt(settings, "maxHitsForQueryPath", maxHitsForQueryPath);
    loadInt(settings, "maxQueryPathNodes", maxQueryPathNodes);
    loadFloat(settings, "minQueryCoveredByPath", minQueryCoveredByPath);
    loadFloat(settings, "minQueryCoveredByHits", minQueryCoveredByHits);
    loadToggle(settings, "minQueryCoveredByHits", minQueryCoveredByHits.on);
    loadFloat(settings, "minMeanHitIdentity", minMeanHitIdentity);
    loadToggle(settings, "minMeanHitIdentity", minMeanHitIdentity.on);
    loadSciNot(settings, "maxEValueProduct", maxEValueProduct);
    loadToggle(settings, "maxEValueProduct", maxEValueProduct.on);
    loadFloat(settings, "minLengthPercentage", minLengthPercentage);
    loadToggle(settings, "minLengthPercentage", minLengthPercentage.on);
    loadFloat(settings, "maxLengthPercentage", maxLengthPercentage);
    loadToggle(settings, "maxLengthPercentage", maxLengthPercentage.on);
    loadInt(settings, "minLengthBaseDiscrepancy", minLengthBaseDiscrepancy);
    loadToggle(settings, "minLengthBaseDiscrepancy", minLengthBaseDiscrepancy.on);
    loadInt(settings, "maxLengthBaseDiscrepancy", maxLengthBaseDiscrepancy);
    loadToggle(settings, "maxLengthBaseDiscrepancy", maxLengthBaseDiscrepancy.on);

    loadInt(settings, "alignmentLengthFilter", alignmentLengthFilter);
    loadToggle(settings, "alignmentLengthFilter", alignmentLengthFilter.on);
    loadFloat(settings, "queryCoverageFilter", queryCoverageFilter);
    loadToggle(settings, "queryCoverageFilter", queryCoverageFilter.on);
    loadFloat(settings, "identityFilter", identityFilter);
    loadToggle(settings, "identityFilter", identityFilter.on);
    loadSciNot(settings, "evalueFilter", evalueFilter);
    loadToggle(settings, "evalueFilter", evalueFilter.on);
    loadFloat(settings, "bitScoreFilter", bitScoreFilter);
    loadToggle(settings, "bitScoreFilter", bitScoreFilter.on);

    const QVariantList palette = settings.value(optionKey("tagColourPalette")).toList();
    if (!palette.isEmpty()) {
        const QVector<QColor> colours = coloursFromVariantList(palette);
        if (!colours.isEmpty())
            tagColourPalette = colours;
    }

    const QVariantMap tagValueMap = settings.value(optionKey("tagValueColours")).toMap();
    if (!tagValueMap.isEmpty()) {
        QHash<QString, QColor> colours;
        for (auto it = tagValueMap.cbegin(); it != tagValueMap.cend(); ++it) {
            const QColor colour = it.value().value<QColor>();
            if (colour.isValid())
                colours.insert(it.key(), colour);
        }
        tagValueColours = colours;
    }

    const QVariantList ruleList = settings.value(optionKey("tagColourRules")).toList();
    if (!ruleList.isEmpty()) {
        QVector<TagColourRule> rules;
        rules.reserve(ruleList.size());
        for (const QVariant &value : ruleList) {
            const QVariantMap ruleMap = value.toMap();
            const int matchType = ruleMap.value("matchType").toInt();
            if (matchType < int(TagMatchType::Equals) || matchType > int(TagMatchType::CsvIncludes))
                continue;

            TagColourRule rule;
            rule.tagName = ruleMap.value("tagName").toString();
            rule.matchType = TagMatchType(matchType);
            rule.value = ruleMap.value("value").toString();
            rule.colour = ruleMap.value("colour").value<QColor>();
            if (!rule.tagName.isEmpty() && rule.colour.isValid())
                rules.append(rule);
        }
        tagColourRules = rules;
    }

    loadValue(settings, "conditionalColour", conditionalColour);
}

void Settings::savePersisted() const
{
    QSettings settings = persistentSettings();

    saveValue(settings, "doubleMode", doubleMode);
    saveEnum(settings, "nodeLengthMode", int(nodeLengthMode));
    saveFloat(settings, "manualNodeLengthPerMegabase", manualNodeLengthPerMegabase);
    saveInt(settings, "graphLayoutQuality", graphLayoutQuality);
    saveValue(settings, "linearLayout", linearLayout);
    saveFloat(settings, "minimumNodeLength", minimumNodeLength);
    saveFloat(settings, "edgeLength", edgeLength);
    saveFloat(settings, "doubleModeNodeSeparation", doubleModeNodeSeparation);
    saveFloat(settings, "nodeSegmentLength", nodeSegmentLength);
    saveFloat(settings, "componentSeparation", componentSeparation);

    saveFloat(settings, "depthEffectOnWidth", depthEffectOnWidth);
    saveFloat(settings, "depthPower", depthPower);
    saveFloat(settings, "edgeWidth", edgeWidth);
    saveFloat(settings, "linkWidth", linkWidth);
    saveFloat(settings, "outlineThickness", outlineThickness);
    saveValue(settings, "arrowheadsInSingleMode", arrowheadsInSingleMode);
    saveFloat(settings, "textOutlineThickness", textOutlineThickness);
    saveValue(settings, "rainbowPartsPerQuery", rainbowPartsPerQuery);

    saveValue(settings, "edgeColour", edgeColour);
    saveValue(settings, "outlineColour", outlineColour);
    saveValue(settings, "selectionColour", selectionColour);
    saveValue(settings, "textColour", textColour);
    saveValue(settings, "textOutlineColour", textOutlineColour);
    saveValue(settings, "uniformPositiveNodeColour", uniformPositiveNodeColour);
    saveValue(settings, "uniformNegativeNodeColour", uniformNegativeNodeColour);
    saveValue(settings, "uniformNodeSpecialColour", uniformNodeSpecialColour);

    saveInt(settings, "randomColourPositiveOpacity", randomColourPositiveOpacity);
    saveInt(settings, "randomColourNegativeOpacity", randomColourNegativeOpacity);
    saveInt(settings, "randomColourPositiveSaturation", randomColourPositiveSaturation);
    saveInt(settings, "randomColourNegativeSaturation", randomColourNegativeSaturation);
    saveInt(settings, "randomColourPositiveLightness", randomColourPositiveLightness);
    saveInt(settings, "randomColourNegativeLightness", randomColourNegativeLightness);

    saveInt(settings, "contiguitySearchSteps", contiguitySearchSteps);
    saveValue(settings, "contiguousStrandSpecificColour", contiguousStrandSpecificColour);
    saveValue(settings, "contiguousEitherStrandColour", contiguousEitherStrandColour);
    saveValue(settings, "maybeContiguousColour", maybeContiguousColour);
    saveValue(settings, "notContiguousColour", notContiguousColour);
    saveValue(settings, "contiguityStartingColour", contiguityStartingColour);

    saveValue(settings, "grayColor", grayColor);
    saveValue(settings, "autoDepthValue", autoDepthValue);
    saveFloat(settings, "lowDepthValue", lowDepthValue);
    saveFloat(settings, "highDepthValue", highDepthValue);
    saveEnum(settings, "colorMap", int(colorMap));

    saveValue(settings, "pathHighlightShadingColour", pathHighlightShadingColour);
    saveValue(settings, "pathHighlightOutlineColour", pathHighlightOutlineColour);
    saveValue(settings, "defaultCustomNodeColour", defaultCustomNodeColour);

    saveValue(settings, "displayNodeNames", displayNodeNames);
    saveValue(settings, "displayNodeLengths", displayNodeLengths);
    saveValue(settings, "displayNodeDepth", displayNodeDepth);
    saveValue(settings, "labelFont", labelFont);
    saveValue(settings, "textOutline", textOutline);
    saveValue(settings, "antialiasing", antialiasing);
    saveValue(settings, "positionTextNodeCentre", positionTextNodeCentre);

    saveEnum(settings, "graphScope", int(graphScope));
    saveInt(settings, "nodeDistance", nodeDistance);
    saveValue(settings, "startingNodesExactMatch", startingNodesExactMatch);
    saveValue(settings, "startingNodes", startingNodes);
    saveFloat(settings, "minDepthRange", minDepthRange);
    saveFloat(settings, "maxDepthRange", maxDepthRange);

    saveValue(settings, "searchParameters", searchParameters);

    saveInt(settings, "maxHitsForQueryPath", maxHitsForQueryPath);
    saveInt(settings, "maxQueryPathNodes", maxQueryPathNodes);
    saveFloat(settings, "minQueryCoveredByPath", minQueryCoveredByPath);
    saveFloat(settings, "minQueryCoveredByHits", minQueryCoveredByHits);
    saveToggle(settings, "minQueryCoveredByHits", minQueryCoveredByHits.on);
    saveFloat(settings, "minMeanHitIdentity", minMeanHitIdentity);
    saveToggle(settings, "minMeanHitIdentity", minMeanHitIdentity.on);
    saveSciNot(settings, "maxEValueProduct", maxEValueProduct);
    saveToggle(settings, "maxEValueProduct", maxEValueProduct.on);
    saveFloat(settings, "minLengthPercentage", minLengthPercentage);
    saveToggle(settings, "minLengthPercentage", minLengthPercentage.on);
    saveFloat(settings, "maxLengthPercentage", maxLengthPercentage);
    saveToggle(settings, "maxLengthPercentage", maxLengthPercentage.on);
    saveInt(settings, "minLengthBaseDiscrepancy", minLengthBaseDiscrepancy);
    saveToggle(settings, "minLengthBaseDiscrepancy", minLengthBaseDiscrepancy.on);
    saveInt(settings, "maxLengthBaseDiscrepancy", maxLengthBaseDiscrepancy);
    saveToggle(settings, "maxLengthBaseDiscrepancy", maxLengthBaseDiscrepancy.on);

    saveInt(settings, "alignmentLengthFilter", alignmentLengthFilter);
    saveToggle(settings, "alignmentLengthFilter", alignmentLengthFilter.on);
    saveFloat(settings, "queryCoverageFilter", queryCoverageFilter);
    saveToggle(settings, "queryCoverageFilter", queryCoverageFilter.on);
    saveFloat(settings, "identityFilter", identityFilter);
    saveToggle(settings, "identityFilter", identityFilter.on);
    saveSciNot(settings, "evalueFilter", evalueFilter);
    saveToggle(settings, "evalueFilter", evalueFilter.on);
    saveFloat(settings, "bitScoreFilter", bitScoreFilter);
    saveToggle(settings, "bitScoreFilter", bitScoreFilter.on);

    settings.setValue(optionKey("tagColourPalette"), coloursToVariantList(tagColourPalette));

    QVariantMap tagValueMap;
    for (auto it = tagValueColours.cbegin(); it != tagValueColours.cend(); ++it)
        tagValueMap.insert(it.key(), it.value());
    settings.setValue(optionKey("tagValueColours"), tagValueMap);

    QVariantList ruleList;
    ruleList.reserve(tagColourRules.size());
    for (const TagColourRule &rule : tagColourRules) {
        QVariantMap ruleMap;
        ruleMap.insert("tagName", rule.tagName);
        ruleMap.insert("matchType", int(rule.matchType));
        ruleMap.insert("value", rule.value);
        ruleMap.insert("colour", rule.colour);
        ruleList.append(ruleMap);
    }
    settings.setValue(optionKey("tagColourRules"), ruleList);

    saveValue(settings, "conditionalColour", conditionalColour);
}
