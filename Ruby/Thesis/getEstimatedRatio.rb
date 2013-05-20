#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
require File.dirname(__FILE__) + "/paramGenThesis.rb"

STDOUT.sync = true

res_str=""

#X:\result_master\3disEst\shinpukan-master201202\N5\random0\BoVWSIFT_histIntsec\p1_cam105-110.11.05\cam105-106\
#estimationResult_BoVWSIFT_histIntsec_p1_cam105-110.11.05_cam105-106.txt
cam_pair_cluster_condIdxLst=Cam_pair_cluster_condIdxLst
capAlpLst=CapAlpLst
plotEstMethodStrLst=PlotEstMethodLst
featIdxLst=FeatIdxLst
setID=SetID
featIdxLst.each_with_index{|fidx,tabIdx|#1つの表
str = <<"EOS"
\\begin{table}[htb] %resultsRatioEst
\\begin{center}
\\caption{\\captionRatioEst #{FeatLst[fidx]}}
\\begin{tabular}{|c||c|c|c|c|}
\\hline
EOS
res_str+=str
res_str+="\\error / \\camera"
cam_pair_cluster_condIdxLst.size.times{|cidx|#1列
#cam_pair_cluster_condIdxLst.each{|cam_pairIdx,cluster_condIdx|
	cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
	cam_pair=CamPairLst[cam_pairIdx]
	cluster_cond=Cluster_condLst[cluster_condIdx+1]
	cam_pair_str=getCamPairStr(cam_pair).gsub('camera','')
	res_str+=" & #{cam_pair_str}"
}
res_str+="\\\\ \\hline\n"
#カメラ1＼カメラ2 & カメラA & カメラB & カメラC & カメラD & カメラE & カメラF\\ \hline
	PlotEstMethodLst.each_with_index{|estMethodStr,eidx|#1行
		res_str+="$error_#{eidx+1}$"
		cam_pair_cluster_condIdxLst.size.times{|cidx|#1列
		#cam_pair_cluster_condIdxLst.each{|cam_pairIdx,cluster_condIdx|
			cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
			cam_pair=CamPairLst[cam_pairIdx]
			cluster_cond=Cluster_condLst[cluster_condIdx+1]
			cam_pair_str=getCamPairStr(cam_pair)#.gsub('cam','camera')
			estResultDir=MasterResultEstDir+"N#{N}/"+"#{setID}/"+"#{FeatureMetrics_ParamLst[fidx][0]}/"+"#{FeatureMetrics_ParamLst[fidx][1]}/"+"#{cam_pair}/"
			estResultFname="estimationResult_#{FeatureMetrics_ParamLst[fidx][0]}_#{FeatureMetrics_ParamLst[fidx][1]}_#{cam_pair}.txt"
			p estResultFname
			estArray=(estResultDir+estResultFname).getArray
			tempRow=estArray.select{|str|str.index(estMethodStr)!=nil && str.index(cluster_cond)!=nil}
			raise ArgumentError, "" if tempRow.size!=1
			estimatedRatioStr=tempRow[0].split(" ")[2]
			res_str+= " & " + estimatedRatioStr[0..5]
		}#cam 行終わり
		res_str+="\\\\ \\hline\n"
	}#estMethod 表終わり
str = <<"EOS"
\\end{tabular}
\\label{tbl:estimatedRatio#{tabIdx}}
\\end{center}
\\end{table}
EOS
res_str+=str
}#feat
puts res_str
