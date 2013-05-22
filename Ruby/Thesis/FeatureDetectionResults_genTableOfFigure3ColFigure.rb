#!/usr/bin/ruby
# require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/Utils.rb"
# require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
# require File.dirname(__FILE__) + "/paramGenThesis.rb"
# @brief �e�摜�E�e�������o�A���S���Y���E�e�p�����[�^�����[�v����tex�\���p�̕\���쐬����B
# @note tex�t�@�C���ł������錾���Ă����}�N������i���{��e�L�X�g�u�������p)
# @note �p�����[�^�̎擾�Ƃ��A���K�\���ł������菑�������Ƃ��������B



STDOUT.sync = true
res_str=""

# �摜���Ƃ̃��[�v
ImgLst=["kaba" , "lena" ,  "nature"]
# ImgDir="C:/Users/ZQ83219/Documents/Visual\ Studio\ 2008/Projects/SandBox/FeatureExtractionTest/FeatureDetectionTest/output"
ImgDir="output"
# imgName = "kaba"
# imgIdx=0

FeatureLst=["SIFT" , "SURF" , "STAR" , "ORB" , "FAST" , "MSER"]
# featureParamLstLst �̗v�f�̕\�L�́A�e�����̒��o���ʂɂ��Ă܂Ƃ߂��t�@�C���̕\�L�ƈ�v�����邱��

FeatureParamLstLst=[
	["octave" , "edgeThreshold" , "contrastThreshold" , "sigma"],
	["hessianThreshold" , "octave" , "octaveLayers"],
	["maxSize" , "responseThreshold" , "lineThresholdProjected" , "lineThresholdBinarized" , "supressNonMaxSize"],
	["scaleFactor" , "nlevels" , "edgeThreshold" , "firstLevel" , "WTAK" , "scoreType", "patchSize"],
	["threshold" , "nonmaxSuppression"],
	["delta" , "minArea" , "maxArea" , "maxVariation" , "minDiversity" , "maxEvolution" , "areaThreshold" , "minMargin" , "edgeBlurSize"]
]
# �������Ƃ̃��[�v
# FeatureLst.each_with_index{|featureName, featIdx|
featIdx = 5
featureName = FeatureLst[featIdx]
	# �p�����[�^���Ƃ̃��[�v
	FeatureParamLstLst[featIdx].each_with_index{ |paramName, paramIdx|
		ImgLst.each_with_index{|imgName, imgIdx|
		# paramName = "edgeThreshold"
		# paramIdx = 0
			#�}�\�̓p�����[�^���Ƃ�1���
			# #--�}�J�n
			# res_str+=""
			#puts res_str
			#�p�����[�^�̎�肤��l�̃��X�g���@ls �Ŏ擾
			detectionResultDir="#{ImgDir}/#{imgName}/#{featureName}/#{paramName}"
			cmd = "ls #{detectionResultDir} | grep .eps"
			# p "#{cmd}"
			featureParamValImgNameLst=`#{cmd}`.split("\n")
			# p featureParamValImgNameLst
#--�\�n�߂��L�q
str = <<"EOS"
\\begin{figure}[htpb] %--detection_results_#{featureName}_#{paramName}
\\begin{center}
EOS
			paramVal = 0 #�X�R�[�v�������܂Ŋm��
			res_str += str
 			0.upto(featureParamValImgNameLst.size - 1){|paramValIdx|
				#�p�����[�^�̒l���Ƃ̃��[�v(1�Z�����Ƃɐi��)
 				paramValImgName = featureParamValImgNameLst[paramValIdx]

				# ��J�n�@paramVal�𒲂ׂ�-------------------------------------------------------
				paramValLstLstFilepath = "#{detectionResultDir}/TimeLog_#{imgName}_#{featureName}_#{paramName}.txt"
				paramValLstLst=paramValLstLstFilepath.getArray
				# threshold-010_nonmaxSupression-0 58249points 4msec �̃t�H�[�}�b�g�𕪉�
				isParamValSet = false
				paramNumOfKeypointsStr = paramValLstLst[paramValIdx].split(" ")[1]
				paramTimeStr = paramValLstLst[paramValIdx].split(" ")[2]
				paramValLstLst[paramValIdx].split(" ")[0].split("_").each{ |paramValPairStr| 
					# paramValPairStr: param-val�̌`��string
					paramValPair = paramValPairStr.split("-")
					if paramValPair[0] == paramName then
						paramVal = paramValPair[1].to_f
						# p paramVal
						isParamValSet = true
						break
					end
				}
				if isParamValSet == false
					p "paramVal not found: #{paramName} in #{paramValLstLstFilepath}"
				end
				#-----------------------------------------------------------------------------
# p "paramVal:"
# p paramVal
str = <<"EOS"
\\begin{minipage}[b]{0.32\\textwidth}
\\begin{center}
%widthOfResults#{featureName} defined in .tex file
\\includegraphics[width=\\widthOfResults#{imgName}]{#{detectionResultDir}/#{paramValImgName}}\\\\
(#{paramValIdx}) #{paramName} = #{paramVal}\\\\
detect #{paramNumOfKeypointsStr} in #{paramTimeStr}
\\end{center}
\\end{minipage}
EOS
				res_str+=str
				if paramValIdx % 3 == 2 then
					res_str += "\\\\\n" # ���s�}��
				end
			}#featureParamValLst.each
#�\�I���L�q
str = <<"EOS"
	\\end{center}
	\\vskip-\\lastskip
	%caption#{featureName} defined in .tex file
	\\caption{#{featureName} under #{paramName} change}
	\\label{fig:results_#{imgName}_#{featureName}_#{paramName}}
	\\end{figure}
EOS
			res_str+=str
		}#ImgLst.each
		res_str+="\\clearpage\n"
	}#FeatureParamLst.each
# }#FeatureLst.each
#�\�̐����������邽�߁Afloat�������[�����
puts res_str