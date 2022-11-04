import 'dart:convert';
import 'dart:html';

import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  static const String _title = 'Touch Data Collection: Perform a gesture on the screen then click the download button.';

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: _title,
      home: Scaffold(
        appBar: AppBar(title: const Text(_title)),
        body: const Center(
          child: MyStatefulWidget(),
        ),
      ),
    );
  }
}

class MyStatefulWidget extends StatefulWidget {
  const MyStatefulWidget({super.key});

  @override
  State<MyStatefulWidget> createState() => _MyStatefulWidgetState();
}

class _MyStatefulWidgetState extends State<MyStatefulWidget> {
  final List<String> _details = List.empty(growable: true);

  void _processPointerDown(PointerEvent details) {
    _printDetails(details, "down");
  }

  void _processPointerUp(PointerEvent details) {
    _printDetails(details, "up");
  }

  void _processPointerMove(PointerEvent details) {
    _printDetails(details, "move");
  }

  void _printDetails(PointerEvent details, String type) {
    List<String> detailsString = [
      type,
      "${details.down}",
      "${details.position.dx}",
      "${details.position.dy}",
      "${details.timeStamp}",
      "${details.distance}",
      "${details.distanceMax}",
      "${details.distanceMin}",
      "${details.kind}",
      "${details.orientation}",
      "${details.pressure}",
      "${details.pressureMax}",
      "${details.pressureMin}",
      "${details.radiusMajor}",
      "${details.radiusMinor}",
      "${details.radiusMax}",
      "${details.radiusMin}",
      "${details.size}",
      "${details.tilt}",
    ];
    _details.add(detailsString.join(','));
  }

  void _downloadDetails() {
    List<String> header = [
      "type",
      "down",
      "position.dx",
      "position.dy",
      "timeStamp",
      "distance",
      "distanceMax",
      "distanceMin",
      "kind",
      "orientation",
      "pressure",
      "pressureMax",
      "pressureMin",
      "radiusMajor",
      "radiusMinor",
      "radiusMax",
      "radiusMin",
      "size",
      "tilt",
    ];
    _details.insert(0, header.join(','));
    String contents = "${_details.join("\n")}\n";
    List<int> bytes = utf8.encode(contents);
    String encoded = base64Encode(bytes);
    AnchorElement anchor = AnchorElement(href: "data:application/octet-stream;charset=utf-8;base64,$encoded");
    anchor.setAttribute("download", "gesture_${DateTime.now().toIso8601String()}.csv");
    anchor.click();
    _details.clear();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Listener(
          onPointerDown: _processPointerDown,
          onPointerMove: _processPointerMove,
          onPointerUp: _processPointerUp,
          child: Container(
            color: Colors.lightBlueAccent,
          ),
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _downloadDetails,
        tooltip: "Download",
        child: const Icon(Icons.download),
      ),
    );
  }
}
