import bb.cascades 1.3
import bb.system 1.0

Page {
    id: root

    // TODO: use enum
    // 0 = stopped
    // 1 = recording
    // 2 = paused
    property int state: 0

    onCreationCompleted: {
        _geoLocation.error.connect(statusLabel.setError)
        _geoLocation.warning.connect(statusLabel.setWarning)
        _geoLocation.setPeriod(5)
        _geoLocation.startUpdates()
        Application.autoExit = true
        Application.aboutToQuit.connect(_gpxFile.close)
        Application.fullscreen.connect(onFullscreen)
        Application.thumbnail.connect(onThumbnail)
    }

    function onFullscreen() {
        _geoLocation.dataChanged.connect(onDataChangedUi)
    }

    function onThumbnail() {
        _geoLocation.dataChanged.disconnect(onDataChangedUi)
    }

    function onDataChangedUi() {
        latLabel.setTextFrom(_geoLocation.latitude)
        lonLabel.setTextFrom(_geoLocation.longitude)
        accLabel.setTextFrom(_geoLocation.accuracy)
        speedLabel.setTextFrom(_geoLocation.speed)
        altLabel.setTextFrom(_geoLocation.altitude)
        vertAccLabel.setTextFrom(_geoLocation.vertAccuracy)
        vertSpeedLabel.setTextFrom(_geoLocation.vertSpeed)
        numSatLabel.setTextFrom(_geoLocation.numSatellitesUsed, _geoLocation.numSatellitesTotal)
        if (!isNaN(_geoLocation.latitude) &&
            !isNaN(_geoLocation.longitude))
        {
            statusLabel.clearStatus()
        }
    }

    function onDataChangedFile() {
        if (isNaN(_geoLocation.latitude) ||
            isNaN(_geoLocation.longitude))
        {
            return
        }
        _gpxFile.writeStartTrackPoint(_geoLocation.latitude, _geoLocation.longitude)
        if (!isNaN(_geoLocation.altitude)) _gpxFile.writeAltitude(_geoLocation.altitude)
        _gpxFile.writeTimestamp(_geoLocation.timestamp)
        if (!isNaN(_geoLocation.geoidHeight)) _gpxFile.writeGeoidHeight(_geoLocation.geoidHeight)
        if (_geoLocation.numSatellitesUsed > 0) _gpxFile.writeNumSatellitesUsed(_geoLocation.numSatellitesUsed)
        if (!isNaN(_geoLocation.hdop)) _gpxFile.writeHdop(_geoLocation.hdop)
        if (!isNaN(_geoLocation.vdop)) _gpxFile.writeVdop(_geoLocation.vdop)
        if (!isNaN(_geoLocation.pdop)) _gpxFile.writePdop(_geoLocation.pdop)
        _gpxFile.writeStartExtensions()
        if (!isNaN(_geoLocation.heading)) _gpxFile.writeTextElement("hdg", _geoLocation.heading.toFixed(1))
        if (!isNaN(_geoLocation.speed)) _gpxFile.writeTextElement("s", _geoLocation.speed.toFixed(1))
        if (!isNaN(_geoLocation.vertSpeed)) _gpxFile.writeTextElement("vs", _geoLocation.vertSpeed.toFixed(1))
        if (!isNaN(_geoLocation.accuracy)) _gpxFile.writeTextElement("acc", _geoLocation.accuracy.toFixed(2))
        if (!isNaN(_geoLocation.vertAccuracy)) _gpxFile.writeTextElement("vacc", _geoLocation.vertAccuracy.toFixed(2))
        _gpxFile.writeEndExtensions()
        _gpxFile.writeEndTrackPoint()
    }

    attachedObjects: [
        SystemToast {
            id: saveToast
        }
    ]

    actions: [
        ActionItem {
            title: qsTr("Pause")
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: state == 1
            onTriggered: {
                _geoLocation.dataChanged.disconnect(root.onDataChangedFile)
                _gpxFile.writeComment("pause")
                state = 2
            }
            imageSource: "asset:///images/ic_pause.png"
        },
        ActionItem {
            title: qsTr("Record")
            ActionBar.placement: ActionBarPlacement.Signature
            enabled: state != 1
            onTriggered: {
                if (state == 0) {
                    // Initial start
                    var fileName = Qt.formatDateTime(new Date, "yyyyMMdd_HHmmss") + ".gpx"
                    saveToast.body = fileName
                    _gpxFile.open(fileName)
                    Application.setClosePrompt(qsTr("Recording is active"),
                        qsTr("The application records location data to a GPX file. File will be finalized and closed."))
                } else if (state == 2) {
                    // Resume after pause
                }
                _geoLocation.dataChanged.connect(root.onDataChangedFile)
                state = 1
            }
            imageSource: "asset:///images/ic_location.png"
        },
        ActionItem {
            title: qsTr("Stop")
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: state != 0
            onTriggered: {
                _geoLocation.dataChanged.disconnect(root.onDataChangedFile)
                _gpxFile.close()
                saveToast.show()
                Application.clearClosePrompt()
                state = 0
            }
            imageSource: "asset:///images/ic_stop.png"
        }
    ]

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topPadding: ui.du(2.0)
        leftPadding: topPadding
        rightPadding: leftPadding
        bottomPadding: topPadding
        Header {
            title: qsTr("Location and Speed")
        }
        Label {
            id: latLabel
            textStyle.fontSize: FontSize.XXLarge
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.textAlign: TextAlign.Center
            textStyle.fontFamily: "Monospace"
            function setTextFrom(deg) {
                text = (isNaN(deg) ? "N/A" : deg.toFixed(8)) + "°"
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Label {
            id: lonLabel
            textStyle.fontSize: FontSize.XXLarge
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.textAlign: TextAlign.Center
            textStyle.fontFamily: "Monospace"
            function setTextFrom(deg) {
                text = (isNaN(deg) ? "N/A" : deg.toFixed(8)) + "°"
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Label {
            id: accLabel
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(acc) {
                text = "±" + (isNaN(acc) ? "N/A" : acc.toFixed(2)) + " " + qsTr("m")
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Label {
            id: speedLabel
            textStyle.fontSize: FontSize.Large
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(s) {
                text = (isNaN(s) ? "N/A" : s.toFixed(1)) + " " + qsTr("m/s")
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Header {
            title: qsTr("Altitude and Vertical Speed")
        }
        Label {
            id: altLabel
            textStyle.fontSize: FontSize.Large
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(alt) {
                text = (isNaN(alt) ? "N/A" : alt.toFixed(2)) + " " + qsTr("m")
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Label {
            id: vertAccLabel
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(vacc) {
                text = "±" + (isNaN(vacc) ? "N/A" : vacc.toFixed(2)) + " " + qsTr("m")
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Label {
            id: vertSpeedLabel
            textStyle.fontSize: FontSize.Large
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(vs) {
                text = (isNaN(vs) ? "N/A" : vs.toFixed(1)) + " " + qsTr("m/s")
            }
            onCreationCompleted: setTextFrom(NaN)
        }
        Header {
            title: qsTr("Number of Satellites")
        }
        Label {
            id: numSatLabel
            textStyle.fontSize: FontSize.Large
            textStyle.textAlign: TextAlign.Center
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.fontFamily: "Monospace"
            function setTextFrom(used, total) {
                text = used + "/" + total
            }
            onCreationCompleted: setTextFrom(0, 0)
        }

        Label {
            id: statusLabel
            horizontalAlignment: HorizontalAlignment.Fill
            textStyle.textAlign: TextAlign.Center
            multiline: true
            function clearStatus() {
                text = undefined
                textStyle.color = undefined
            }
            function setError(err) {
                if (err == "disabled") {
                    text = qsTr("Location services are disabled. Please enable location services and restart the application.")
                } else {
                    text = qsTr("Unknown error in location services. Try to restart the application.")
                }
                textStyle.color = Color.create("#ffff3333")
            }
            function setWarning(warn) {
                if (warn == "timeout") {
                    text = qsTr("No information reported from location services.")
                } else if (warn == "lostTracking") {
                    text = qsTr("Satellite signal lost. Try to get to a place with more sky visible.")
                } else if (warn == "stationary") {
                    text = qsTr("The device is stationary.")
                } else {
                    // Unknown warning
                    return
                }
                textStyle.color = Color.create("#ffdcd427")
            }
        }
    }
}
