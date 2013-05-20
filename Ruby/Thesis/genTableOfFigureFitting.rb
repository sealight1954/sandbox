#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
require File.dirname(__FILE__) + "/paramGenThesis.rb"
STDOUT.sync = true

#cam_pairIdxLst=[4,12,15,22,24,34]
featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]
setID="random0"
featIdxLst=FeatIdxLst
cam_pairIdxLst=Cam_pairIdxLst
res_str=""
cluster_cond=Cluster_condLst[1]
featureMetricsStr="#{FeatureMetrics_ParamLst[0][0]}_#{FeatureMetrics_ParamLst[0][1]}"
results_folder=Results_folder
capAlpLst=CapAlpLst
#--------------------------------------------------------------------------------
	#--図開始
str = <<'EOS'
\begin{figure}[htpb]
\begin{center}
EOS
res_str+=str
#カメラごとのループ
#cam_pairIdxLst.each{|cidx|


	#puts res_str
	#--行開始
	#--列開始
	#featIdxLst.each_with_index{|fidx,col|
idx=0
2.times{|row|
	2.times{|i|
		cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
		cam_pair=CamPairLst[cam_pairIdx]
		cluster_cond=Cluster_condLst[cluster_condIdx+1]
		cam_pair_str=getCamPairStr(cam_pair)#cam_pair.gsub('cam','camera')
str = <<"EOS"
	\\begin{minipage}[b]{0.49\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsDouble]{#{results_folder}/Fitting/param-Fitting#{cluster_condIdx}_#{setID}_#{cluster_cond}_#{featureMetricsStr}_#{cam_pair}.eps}\\\\
	(#{capAlpLst[idx]}) #{cam_pair_str} cluster#{i+1}
	\\end{center}
	\\end{minipage}
EOS
		res_str+=str
		idx+=1
	}#--列終わり
	res_str+="\\\\\n" if row!=2
}#--行終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{--str--}
\\label{fig:resultsPositive}
\\end{figure}
EOS
res_str+=str
#図終わり
puts res_str
