#!/usr/bin/ruby
require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/../lib/Utils.rb"
require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
STDOUT.sync = true

cam_pairIdxLst=[4,12,15,22,24,34]
#cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
cam_pair_cluster_condIdxLst=[[7,0],[31,0],[23,1],[28,1]]
#cam_pair_cluster_condIdxLst=[[0,0],[12,1],[16,1],[19,0],[26,0],[30,0]]
featLst=["BoVW SIFT","RGB Histogram","SDALF wHSV"]
res_str=""
capAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
cluster_cond=Cluster_condLst[1]
param="Likelihood-clustering"
param="Likelihood"

#featureMetricsStr="#{FeatureMetrics_ParamLst[0][0]}_#{FeatureMetrics_ParamLst[0][1]}"
setID="random0"
featureWeight="BMW"
estMethodStr=EstMethodStr
results_folder=Results_folder
def getCamPairStr(cam_pair)
	cam_pair_str=cam_pair.gsub('cam','camera')
		[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F '],['111',' A ']].each{|cam,cam_str|
		cam_pair_str=cam_pair_str.gsub(cam,cam_str)
	}
	return cam_pair_str
end

#--------------------------------------------------------------------------------
	#--図開始
str = <<"EOS"
\\begin{figure}[htpb]%--results#{param}--------------------------------------
\\begin{center}
EOS
res_str+=str

cidx=0
2.times{|row|
	2.times{|i|
	cam_pairIdx,cluster_condIdx=cam_pair_cluster_condIdxLst[cidx]
	cam_pair=CamPairLst[cam_pairIdx]
	cluster_cond=Cluster_condLst[cluster_condIdx+1]
	cam_pair_str=getCamPairStr(cam_pair)#cam_pair.gsub('cam','camera')
	#[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F '],['111',' A ']].each{|cam,cam_str|cam_pair_str=cam_pair_str.gsub(cam,cam_str)}
		#likelihood
		#param-Likelihood0_knownR_cam105-106_RBW_RGBHistkmeans2Median0_random0.jpg
		#param-Likelihood-clustering0_knownR_cam110-105_RBW_RGBHistkmeans2Median0_random0.jpg
str = <<"EOS"
	\\begin{minipage}[b]{0.49\\textwidth}
	\\begin{center}
	\\includegraphics[width=\\widthOfResultsLikelihood]{#{results_folder}/#{param}/param-#{param}#{cluster_condIdx}_#{estMethodStr}_#{cam_pair}_#{featureWeight}_#{Cluster_condLst[cluster_condIdx+1]}_#{setID}.eps}\\\\
	(#{capAlpLst[cidx]}) #{cam_pair_str}
	\\end{center}
	\\end{minipage}
EOS
		res_str+=str
		cidx+=1
	}#--列終わり
	res_str+="\\\\\n" if row!=2
}#--行終わり
str = <<"EOS"
\\end{center}
\\vskip-\\lastskip
\\caption{\\caption#{param.gsub("-","")}}
\\label{fig:results#{param}}
\\end{figure}
EOS
res_str+=str
#図終わり
puts res_str
