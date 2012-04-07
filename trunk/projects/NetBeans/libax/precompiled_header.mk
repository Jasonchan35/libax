all: ${axOBJECT_PATH}/precompiled_header_make.mk

${axOBJECT_PATH}/precompiled_header.h : ../../../src/precompiled_header/precompiled_header.h
	mkdir -p ${axOBJECT_PATH}
	cp ../../../src/precompiled_header/precompiled_header.h ${axOBJECT_PATH}/precompiled_header.h

${axOBJECT_PATH}/precompiled_header.h.d : ${axOBJECT_PATH}/precompiled_header.h
	@echo ==== generate precompile header dep. =====
	g++ ${axCCFLAGS_gch} -c ../../../src/precompiled_header/precompiled_header.h \
	    -MD -MP -MF ${axOBJECT_PATH}/precompiled_header.h.d \
	    -o ${axOBJECT_PATH}/precompiled_header.h.gch

${axOBJECT_PATH}/precompiled_header_make.mk : ${axOBJECT_PATH}/precompiled_header.h.d
#	echo "all: ${axOBJECT_PATH}/precompiled_header.h.gch" > ${axOBJECT_PATH}/precompiled_header_make.mk
	@cat  ${axOBJECT_PATH}/precompiled_header.h.d >> ${axOBJECT_PATH}/precompiled_header_make.mk
	@echo "" >> ${axOBJECT_PATH}/precompiled_header_make.mk
	@echo "${axOBJECT_PATH}/precompiled_header.h.gch:" >> ${axOBJECT_PATH}/precompiled_header_make.mk
	@echo "		@echo ==== generate precompile header =====" >> ${axOBJECT_PATH}/precompiled_header_make.mk
	@echo "		g++ ${axCCFLAGS_gch} -c ../../../src/precompiled_header/precompiled_header.h -MD -MP -MF ${axOBJECT_PATH}/precompiled_header.h.d -o ${axOBJECT_PATH}/precompiled_header.h.gch" >> ${axOBJECT_PATH}/precompiled_header_make.mk
	
