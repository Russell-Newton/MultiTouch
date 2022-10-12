import 'dart:convert';
import 'dart:html';

import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  static const String _title = 'Touch Data Collection';

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

  _MyStatefulWidgetState() {
    _details.add("type,down,position.dx,position.dy,timeStamp");
  }

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
    _details.add("$type,${details.down},${details.position.dx},${details.position.dy},${details.timeStamp}");
  }

  void _downloadDetails() {
    String contents = "${_details.join("\n")}\n";
    List<int> bytes = utf8.encode(contents);
    String encoded = base64Encode(bytes);
    AnchorElement anchor = AnchorElement(href: "data:application/octet-stream;charset=utf-8;base64,$encoded");
    anchor.setAttribute("download", "gesture_${DateTime.now().toIso8601String()}.csv");
    anchor.click();
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        ConstrainedBox(
          constraints: BoxConstraints.tight(const Size(300.0, 400.0)),
          child: Listener(
            onPointerDown: _processPointerDown,
            onPointerMove: _processPointerMove,
            onPointerUp: _processPointerUp,
            child: Container(
              color: Colors.lightBlueAccent,
            ),
          ),
        ),
        TextButton(
            onPressed: _downloadDetails,
            child: const Text("Download"),
        ),
      ],
    );
  }
}
