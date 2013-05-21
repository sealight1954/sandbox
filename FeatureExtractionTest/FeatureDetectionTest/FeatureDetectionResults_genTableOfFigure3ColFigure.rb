#!/usr/bin/ruby
# require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
# require File.dirname(__FILE__) + "/../lib/Utils.rb"
# require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
# require File.dirname(__FILE__) + "/paramGenThesis.rb"

STDOUT.sync = true
#--3�����~�J�����y�A���̐}�̃R�[�h
#cam_pairIdxLst=[0,12,16,19,26,30]
#cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
# cam_pair_cluster_condIdxLst=Cam_pair_cluster_condIdxLst
# featIdxLst=FeatIdxLst
# featLst=FeatLst
# setID=SetID
# results_folder=Results_folder
res_str=""
# param="Clustering"
# param="Negative"
# param="Fitting"
# param="EstNegative"
# param="Positive"
#capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
# capAlpLst=CapAlpLst

#�J�������Ƃ̃��[�v
# cam_pair_cluster_condIdxLst.size.times{|cidx|
# #cam_pair_cluster_condIdxLst.each{|cam_pairIdx,cluster_condIdx|
# 	cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
# 	cam_pair=CamPairLst[cam_pairIdx]
# 	cluster_cond=Cluster_condLst[cluster_condIdx+1]
# 	cam_pair_str=getCamPairStr(cam_pair)#.gsub('cam','camera')

# �摜���Ƃ̃��[�v
ImgLst=["kaba" , "lena" ,  "natureMedium"]
# ImgDir="C:/Users/ZQ83219/Documents/Visual\ Studio\ 2008/Projects/SandBox/FeatureExtractionTest/FeatureDetectionTest/output"
ImgDir="output"
ImgLst.each_with_index{|imgName, imgIdx|


	FeatureLst=["SIFT" , "SURF" , "STAR" , "ORB" , "FAST" , "MSER"]
	# featureParamLstLst �̗v�f�̕\�L�́A�e�����̒��o���ʂɂ��Ă܂Ƃ߂��t�@�C���̕\�L�ƈ�v�����邱��

	FeatureParamLstLst=[
		["edgeThreshold" , "contrastThreshold" , "sigma"],
		["hessianThreshold" , "octave" , "octaveLayers"]
	]
	# �������Ƃ̃��[�v
	FeatureLst.each_with_index{|featureName, featIdx|
		# �p�����[�^���Ƃ̃��[�v
		FeatureParamLstLst[featIdx].each_with_index{ |paramName, paramIdx|
	
			#�}�\�̓p�����[�^���Ƃ�1���
			# #--�}�J�n
			# res_str+=""
			#puts res_str
			#�p�����[�^�̎�肤��l�̃��X�g���@ls �Ŏ擾
			detectionResultDir="#{ImgDir}/#{imgName}/#{featureName}/#{paramName}"
			cmd = "ls #{detectionResultDir} | grep .eps"
			featureParamValImgNameLst=`#{cmd}`.split("\n")
			p featureParamValImgNameLst
#--�\�n�߂��L�q
str = <<"EOS"
\\begin{figure}[htpb] %--detection_results_#{featureName}_#{paramName}
\\begin{center}
EOS
					res_str += str
 			0.upto(featureParamValImgNameLst.size - 1){|paramValIdx|
 				paramValImgName = featureParamValImgNameLst[paramValIdx]
 				paramVal = 0
				#�p�����[�^�̒l���Ƃ̃��[�v(1�Z�����Ƃɐi��)
				# if paramValIdx % 3 == 0 then
				# end
# ��J�n�@paramVal�𒲂ׂ�-------------------------------------------------------
str = <<"EOS"
\\begin{minipage}[b]{0.32\\textwidth}
\\begin{center}
%widthOfResults#{featureName} defined in .tex file
\\includegraphics[width=\\widthOfResults#{featureName}]{#{detectionResultDir}/#{paramValImgName}}\\\\
(#{paramValIdx}) #{paramVal}
\\end{center}
\\end{minipage}
EOS
				res_str+=str
			if paramValIdx % 3 == 2 then
				res_str += "\n\\\\\n" # ���s�}��
				end
			}#featureParamValLst.each
#�\�I���L�q
str = <<"EOS"
	\\end{center}
	\\vskip-\\lastskip
	\\caption{\\caption#{featureName} - #{paramName}}
	\\label{fig:results#{featureName}_#{paramName}}
	\\end{figure}
EOS
					res_str+=str
		}#FeatureParamLst.each
	}#FeatureLst.each
}#ImgLst.each
puts res_str