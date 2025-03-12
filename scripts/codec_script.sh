#!/bin/bash
# remove the generated directory before running codec generator
rm -rf /workspace/generated/sbe

# run the SBE tool to generate the SBE C++ codec
java --add-opens java.base/jdk.internal.misc=ALL-UNNAMED \
     --add-exports java.base/jdk.internal.misc=ALL-UNNAMED \
     -Dsbe.xinclude.aware=true \
     -Dsbe.generate.ir=true \
     -Dsbe.target.language=Cpp \
     -Dsbe.target.namespace=sbe \
     -Dsbe.output.dir=/workspace/generated \
     -Dsbe.errorLog=yes \
     -jar /workspace/deps/simple-binary-encoding/sbe-all/build/libs/sbe-all-1.35.0-SNAPSHOT.jar \
     /workspace/schemas/FIXSchemas/fix-message-schema.xml
