#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
require File.dirname(__FILE__) + "/paramGenThesis.rb"

STDOUT.sync = true
#--3�����~�J�����y�A���̐}�̃R�[�h
#cam_pairIdxLst=[0,12,16,19,26,30]
#cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
cam_pair_cluster_condIdxLst=Cam_pair_cluster_condIdxLst
featIdxLst=FeatIdxLst
featLst=FeatLst
setID=SetID
results_folder=Results_folder
res_str=""
param="Clustering"
param="Negative"
param="Fitting"
param="EstNegative"
param="Positive"
#capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
capAlpLst=CapAlpLst

#�J�������Ƃ̃��[�v
cam_pair_cluster_condIdxLst.size.times{|cidx|
#cam_pair_cluster_condIdxLst.each{|cam_pairIdx,cluster_condIdx|
	cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
	cam_pair=CamPairLst[cam_pairIdx]
	cluster_cond=Cluster_condLst[cluster_condIdx+1]
	cam_pair_str=getCamPairStr(cam_pair)#.gsub('cam','camera')
#--�}�J�n
#--�s�J�n
str = <<"EOS"
\\begin{figure}[htpb] %--results#{param}
\\begin{center}
EOS
res_str+=str
#puts res_str
#--��J�n
featIdxLst.each_with_index{|fidx,col|
case param
	when "Clustering"#cluster_cond�͊֌W�Ȃ�
		imgName="param-#{param}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cam_pair}.eps"
		caption=featLst[col]
	when "Positive"
	#param-Positive0_random0_RGBHistkmeans2Median0_SDALF-wHSV_f1_cam110-105.jpg
	#param-Positive1_random0_RGBHistkmeans2Median1_SDALF-wHSV_f1_cam108-111.jpg
		imgName="param-#{param}#{cluster_condIdx}_#{setID}_#{cluster_cond}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cam_pair}.eps"
		caption=featLst[col]
	when "Negative"
		imgName="param-#{param}#{cluster_condIdx}_#{cam_pair}_#{setID}_#{cluster_cond}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}.eps"
		caption=featLst[col]
	when "Fitting"
		#param-Fitting0_cam106-107_random0_RGBHistkmeans2Median0_BoVWSIFT_histIntsec_p1_cam105-110.11.05.jpg
		imgName="param-#{param}#{cluster_condIdx}_#{cam_pair}_#{setID}_#{cluster_cond}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}.eps"
		caption=featLst[col]
	when "EstNegative" #�t�B�b�e�B���O����
	#param-EstNegative0_random0_RGBHistkmeans2Median0_SDALF-wHSV_f1_cam110-106.jpg
		imgName="param-#{param}#{cluster_condIdx}_#{setID}_#{cluster_cond}_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cam_pair}.eps"
		caption=featLst[col]
end
str = <<"EOS"
	\\begin{minipage}[b]{0.32\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResults#{param}]{#{results_folder}/#{param}/#{imgName}}\\\\
	(#{capAlpLst[col]}) #{caption}
	\\end{center}
	\\end{minipage}
EOS
res_str+=str
}#--��I���
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{\\caption#{param} #{cam_pair_str}}
\\label{fig:results#{param}_#{cidx}}
\\end{figure}
EOS
res_str+=str
#--�s�I���
#�}�I���
}
puts res_str