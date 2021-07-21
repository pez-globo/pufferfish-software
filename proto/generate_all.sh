#!/bin/bash

echo "Regenerating for MCU..."
./generate_mcu.sh

echo
echo "Regenerating for backend..."
$PROTOC -I=. --python_betterproto_out=../backend/ventserver/protocols/protobuf mcu_pb.proto
$PROTOC -I=. --python_betterproto_out=../backend/ventserver/protocols/protobuf frontend_pb.proto

echo
echo "Regenerating for frontend..."
$PROTOC -I=. --ts_proto_out=../frontend/src/store/proto --plugin="../frontend/node_modules/ts-proto/protoc-gen-ts_proto" --ts_proto_opt=esModuleInterop=true mcu_pb.proto
$PROTOC -I=. --ts_proto_out=../frontend/src/store/proto --plugin="../frontend/node_modules/ts-proto/protoc-gen-ts_proto" --ts_proto_opt=esModuleInterop=true frontend_pb.proto
