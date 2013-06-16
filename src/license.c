/*
  packagemonkey - a package creation assistant
  Copyright (C) 2013  Bob Mottram <bob@robotics.uk.to>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "license.h"

/* saves a LICENSE file for Mozilla */
static void save_license_mozilla(char * filename)
{
	FILE * fp;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp, "%s", "Mozilla Public License Version 2.0\n");
	fprintf(fp, "%s", "==================================\n\n");

	fprintf(fp, "%s", "1. Definitions\n");
	fprintf(fp, "%s", "--------------\n\n");

	fprintf(fp, "%s", "1.1. \"Contributor\"\n");
	fprintf(fp, "%s", "    means each individual or legal entity that creates, contributes to\n");
	fprintf(fp, "%s", "    the creation of, or owns Covered Software.\n\n");

	fprintf(fp, "%s", "1.2. \"Contributor Version\"\n");
	fprintf(fp, "%s", "    means the combination of the Contributions of others (if any) used\n");
	fprintf(fp, "%s", "    by a Contributor and that particular Contributor's Contribution.\n\n");

	fprintf(fp, "%s", "1.3. \"Contribution\"\n");
	fprintf(fp, "%s", "    means Covered Software of a particular Contributor.\n\n");

	fprintf(fp, "%s", "1.4. \"Covered Software\"\n");
	fprintf(fp, "%s", "    means Source Code Form to which the initial Contributor has attached\n");
	fprintf(fp, "%s", "    the notice in Exhibit A, the Executable Form of such Source Code\n");
	fprintf(fp, "%s", "    Form, and Modifications of such Source Code Form, in each case\n");
	fprintf(fp, "%s", "    including portions thereof.\n\n");

	fprintf(fp, "%s", "1.5. \"Incompatible With Secondary Licenses\"\n");
	fprintf(fp, "%s", "    means\n\n");

	fprintf(fp, "%s", "    (a) that the initial Contributor has attached the notice described\n");
	fprintf(fp, "%s", "        in Exhibit B to the Covered Software; or\n\n");

	fprintf(fp, "%s", "    (b) that the Covered Software was made available under the terms of\n");
	fprintf(fp, "%s", "        version 1.1 or earlier of the License, but not also under the\n");
	fprintf(fp, "%s", "        terms of a Secondary License.\n\n");

	fprintf(fp, "%s", "1.6. \"Executable Form\"\n");
	fprintf(fp, "%s", "    means any form of the work other than Source Code Form.\n\n");

	fprintf(fp, "%s", "1.7. \"Larger Work\"\n");
	fprintf(fp, "%s", "    means a work that combines Covered Software with other material, in\n");
	fprintf(fp, "%s", "    a separate file or files, that is not Covered Software.\n\n");

	fprintf(fp, "%s", "1.8. \"License\"\n");
	fprintf(fp, "%s", "    means this document.\n\n");

	fprintf(fp, "%s", "1.9. \"Licensable\"\n");
	fprintf(fp, "%s", "    means having the right to grant, to the maximum extent possible,\n");
	fprintf(fp, "%s", "    whether at the time of the initial grant or subsequently, any and\n");
	fprintf(fp, "%s", "    all of the rights conveyed by this License.\n\n");

	fprintf(fp, "%s", "1.10. \"Modifications\"\n");
	fprintf(fp, "%s", "    means any of the following:\n\n");

	fprintf(fp, "%s", "    (a) any file in Source Code Form that results from an addition to,\n");
	fprintf(fp, "%s", "        deletion from, or modification of the contents of Covered\n");
	fprintf(fp, "%s", "        Software; or\n\n");

	fprintf(fp, "%s", "    (b) any new file in Source Code Form that contains any Covered\n");
	fprintf(fp, "%s", "        Software.\n\n");

	fprintf(fp, "%s", "1.11. \"Patent Claims\" of a Contributor\n");
	fprintf(fp, "%s", "    means any patent claim(s), including without limitation, method,\n");
	fprintf(fp, "%s", "    process, and apparatus claims, in any patent Licensable by such\n");
	fprintf(fp, "%s", "    Contributor that would be infringed, but for the grant of the\n");
	fprintf(fp, "%s", "    License, by the making, using, selling, offering for sale, having\n");
	fprintf(fp, "%s", "    made, import, or transfer of either its Contributions or its\n");
	fprintf(fp, "%s", "    Contributor Version.\n\n");

	fprintf(fp, "%s", "1.12. \"Secondary License\"\n");
	fprintf(fp, "%s", "    means either the GNU General Public License, Version 2.0, the GNU\n");
	fprintf(fp, "%s", "    Lesser General Public License, Version 2.1, the GNU Affero General\n");
	fprintf(fp, "%s", "    Public License, Version 3.0, or any later versions of those\n");
	fprintf(fp, "%s", "    licenses.\n\n");

	fprintf(fp, "%s", "1.13. \"Source Code Form\"\n");
	fprintf(fp, "%s", "    means the form of the work preferred for making modifications.\n\n");

	fprintf(fp, "%s", "1.14. \"You\" (or \"Your\")\n");
	fprintf(fp, "%s", "    means an individual or a legal entity exercising rights under this\n");
	fprintf(fp, "%s", "    License. For legal entities, \"You\" includes any entity that\n");
	fprintf(fp, "%s", "    controls, is controlled by, or is under common control with You. For\n");
	fprintf(fp, "%s", "    purposes of this definition, \"control\" means (a) the power, direct\n");
	fprintf(fp, "%s", "    or indirect, to cause the direction or management of such entity,\n");
	fprintf(fp, "%s", "    whether by contract or otherwise, or (b) ownership of more than\n");
	fprintf(fp, "%s", "    fifty percent (50%) of the outstanding shares or beneficial\n");
	fprintf(fp, "%s", "    ownership of such entity.\n\n");

	fprintf(fp, "%s", "2. License Grants and Conditions\n");
	fprintf(fp, "%s", "--------------------------------\n\n");

	fprintf(fp, "%s", "2.1. Grants\n\n");

	fprintf(fp, "%s", "Each Contributor hereby grants You a world-wide, royalty-free,\n");
	fprintf(fp, "%s", "non-exclusive license:\n\n");

	fprintf(fp, "%s", "(a) under intellectual property rights (other than patent or trademark)\n");
	fprintf(fp, "%s", "    Licensable by such Contributor to use, reproduce, make available,\n");
	fprintf(fp, "%s", "    modify, display, perform, distribute, and otherwise exploit its\n");
	fprintf(fp, "%s", "    Contributions, either on an unmodified basis, with Modifications, or\n");
	fprintf(fp, "%s", "    as part of a Larger Work; and\n\n");

	fprintf(fp, "%s", "(b) under Patent Claims of such Contributor to make, use, sell, offer\n");
	fprintf(fp, "%s", "    for sale, have made, import, and otherwise transfer either its\n");
	fprintf(fp, "%s", "    Contributions or its Contributor Version.\n\n");

	fprintf(fp, "%s", "2.2. Effective Date\n\n");

	fprintf(fp, "%s", "The licenses granted in Section 2.1 with respect to any Contribution\n");
	fprintf(fp, "%s", "become effective for each Contribution on the date the Contributor first\n");
	fprintf(fp, "%s", "distributes such Contribution.\n\n");

	fprintf(fp, "%s", "2.3. Limitations on Grant Scope\n\n");

	fprintf(fp, "%s", "The licenses granted in this Section 2 are the only rights granted under\n");
	fprintf(fp, "%s", "this License. No additional rights or licenses will be implied from the\n");
	fprintf(fp, "%s", "distribution or licensing of Covered Software under this License.\n");
	fprintf(fp, "%s", "Notwithstanding Section 2.1(b) above, no patent license is granted by a\n");
	fprintf(fp, "%s", "Contributor:\n\n");

	fprintf(fp, "%s", "(a) for any code that a Contributor has removed from Covered Software;\n");
	fprintf(fp, "%s", "    or\n\n");

	fprintf(fp, "%s", "(b) for infringements caused by: (i) Your and any other third party's\n");
	fprintf(fp, "%s", "    modifications of Covered Software, or (ii) the combination of its\n");
	fprintf(fp, "%s", "    Contributions with other software (except as part of its Contributor\n");
	fprintf(fp, "%s", "    Version); or\n\n");

	fprintf(fp, "%s", "(c) under Patent Claims infringed by Covered Software in the absence of\n");
	fprintf(fp, "%s", "    its Contributions.\n\n");

	fprintf(fp, "%s", "This License does not grant any rights in the trademarks, service marks,\n");
	fprintf(fp, "%s", "or logos of any Contributor (except as may be necessary to comply with\n");
	fprintf(fp, "%s", "the notice requirements in Section 3.4).\n\n");

	fprintf(fp, "%s", "2.4. Subsequent Licenses\n\n");

	fprintf(fp, "%s", "No Contributor makes additional grants as a result of Your choice to\n");
	fprintf(fp, "%s", "distribute the Covered Software under a subsequent version of this\n");
	fprintf(fp, "%s", "License (see Section 10.2) or under the terms of a Secondary License (if\n");
	fprintf(fp, "%s", "permitted under the terms of Section 3.3).\n\n");

	fprintf(fp, "%s", "2.5. Representation\n\n");

	fprintf(fp, "%s", "Each Contributor represents that the Contributor believes its\n");
	fprintf(fp, "%s", "Contributions are its original creation(s) or it has sufficient rights\n");
	fprintf(fp, "%s", "to grant the rights to its Contributions conveyed by this License.\n\n");

	fprintf(fp, "%s", "2.6. Fair Use\n\n");

	fprintf(fp, "%s", "This License is not intended to limit any rights You have under\n");
	fprintf(fp, "%s", "applicable copyright doctrines of fair use, fair dealing, or other\n");
	fprintf(fp, "%s", "equivalents.\n\n");

	fprintf(fp, "%s", "2.7. Conditions\n\n");

	fprintf(fp, "%s", "Sections 3.1, 3.2, 3.3, and 3.4 are conditions of the licenses granted\n");
	fprintf(fp, "%s", "in Section 2.1.\n\n");

	fprintf(fp, "%s", "3. Responsibilities\n");
	fprintf(fp, "%s", "-------------------\n\n");

	fprintf(fp, "%s", "3.1. Distribution of Source Form\n\n");

	fprintf(fp, "%s", "All distribution of Covered Software in Source Code Form, including any\n");
	fprintf(fp, "%s", "Modifications that You create or to which You contribute, must be under\n");
	fprintf(fp, "%s", "the terms of this License. You must inform recipients that the Source\n");
	fprintf(fp, "%s", "Code Form of the Covered Software is governed by the terms of this\n");
	fprintf(fp, "%s", "License, and how they can obtain a copy of this License. You may not\n");
	fprintf(fp, "%s", "attempt to alter or restrict the recipients' rights in the Source Code\n");
	fprintf(fp, "%s", "Form.\n\n");

	fprintf(fp, "%s", "3.2. Distribution of Executable Form\n\n");

	fprintf(fp, "%s", "If You distribute Covered Software in Executable Form then:\n\n");

	fprintf(fp, "%s", "(a) such Covered Software must also be made available in Source Code\n");
	fprintf(fp, "%s", "    Form, as described in Section 3.1, and You must inform recipients of\n");
	fprintf(fp, "%s", "    the Executable Form how they can obtain a copy of such Source Code\n");
	fprintf(fp, "%s", "    Form by reasonable means in a timely manner, at a charge no more\n");
	fprintf(fp, "%s", "    than the cost of distribution to the recipient; and\n\n");

	fprintf(fp, "%s", "(b) You may distribute such Executable Form under the terms of this\n");
	fprintf(fp, "%s", "    License, or sublicense it under different terms, provided that the\n");
	fprintf(fp, "%s", "    license for the Executable Form does not attempt to limit or alter\n");
	fprintf(fp, "%s", "    the recipients' rights in the Source Code Form under this License.\n\n");

	fprintf(fp, "%s", "3.3. Distribution of a Larger Work\n\n");

	fprintf(fp, "%s", "You may create and distribute a Larger Work under terms of Your choice,\n");
	fprintf(fp, "%s", "provided that You also comply with the requirements of this License for\n");
	fprintf(fp, "%s", "the Covered Software. If the Larger Work is a combination of Covered\n");
	fprintf(fp, "%s", "Software with a work governed by one or more Secondary Licenses, and the\n");
	fprintf(fp, "%s", "Covered Software is not Incompatible With Secondary Licenses, this\n");
	fprintf(fp, "%s", "License permits You to additionally distribute such Covered Software\n");
	fprintf(fp, "%s", "under the terms of such Secondary License(s), so that the recipient of\n");
	fprintf(fp, "%s", "the Larger Work may, at their option, further distribute the Covered\n");
	fprintf(fp, "%s", "Software under the terms of either this License or such Secondary\n");
	fprintf(fp, "%s", "License(s).\n\n");

	fprintf(fp, "%s", "3.4. Notices\n\n");

	fprintf(fp, "%s", "You may not remove or alter the substance of any license notices\n");
	fprintf(fp, "%s", "(including copyright notices, patent notices, disclaimers of warranty,\n");
	fprintf(fp, "%s", "or limitations of liability) contained within the Source Code Form of\n");
	fprintf(fp, "%s", "the Covered Software, except that You may alter any license notices to\n");
	fprintf(fp, "%s", "the extent required to remedy known factual inaccuracies.\n\n");

	fprintf(fp, "%s", "3.5. Application of Additional Terms\n\n");

	fprintf(fp, "%s", "You may choose to offer, and to charge a fee for, warranty, support,\n");
	fprintf(fp, "%s", "indemnity or liability obligations to one or more recipients of Covered\n");
	fprintf(fp, "%s", "Software. However, You may do so only on Your own behalf, and not on\n");
	fprintf(fp, "%s", "behalf of any Contributor. You must make it absolutely clear that any\n");
	fprintf(fp, "%s", "such warranty, support, indemnity, or liability obligation is offered by\n");
	fprintf(fp, "%s", "You alone, and You hereby agree to indemnify every Contributor for any\n");
	fprintf(fp, "%s", "liability incurred by such Contributor as a result of warranty, support,\n");
	fprintf(fp, "%s", "indemnity or liability terms You offer. You may include additional\n");
	fprintf(fp, "%s", "disclaimers of warranty and limitations of liability specific to any\n");
	fprintf(fp, "%s", "jurisdiction.\n\n");

	fprintf(fp, "%s", "4. Inability to Comply Due to Statute or Regulation\n");
	fprintf(fp, "%s", "---------------------------------------------------\n\n");

	fprintf(fp, "%s", "If it is impossible for You to comply with any of the terms of this\n");
	fprintf(fp, "%s", "License with respect to some or all of the Covered Software due to\n");
	fprintf(fp, "%s", "statute, judicial order, or regulation then You must: (a) comply with\n");
	fprintf(fp, "%s", "the terms of this License to the maximum extent possible; and (b)\n");
	fprintf(fp, "%s", "describe the limitations and the code they affect. Such description must\n");
	fprintf(fp, "%s", "be placed in a text file included with all distributions of the Covered\n");
	fprintf(fp, "%s", "Software under this License. Except to the extent prohibited by statute\n");
	fprintf(fp, "%s", "or regulation, such description must be sufficiently detailed for a\n");
	fprintf(fp, "%s", "recipient of ordinary skill to be able to understand it.\n\n");

	fprintf(fp, "%s", "5. Termination\n");
	fprintf(fp, "%s", "--------------\n\n");

	fprintf(fp, "%s", "5.1. The rights granted under this License will terminate automatically\n");
	fprintf(fp, "%s", "if You fail to comply with any of its terms. However, if You become\n");
	fprintf(fp, "%s", "compliant, then the rights granted under this License from a particular\n");
	fprintf(fp, "%s", "Contributor are reinstated (a) provisionally, unless and until such\n");
	fprintf(fp, "%s", "Contributor explicitly and finally terminates Your grants, and (b) on an\n");
	fprintf(fp, "%s", "ongoing basis, if such Contributor fails to notify You of the\n");
	fprintf(fp, "%s", "non-compliance by some reasonable means prior to 60 days after You have\n");
	fprintf(fp, "%s", "come back into compliance. Moreover, Your grants from a particular\n");
	fprintf(fp, "%s", "Contributor are reinstated on an ongoing basis if such Contributor\n");
	fprintf(fp, "%s", "notifies You of the non-compliance by some reasonable means, this is the\n");
	fprintf(fp, "%s", "first time You have received notice of non-compliance with this License\n");
	fprintf(fp, "%s", "from such Contributor, and You become compliant prior to 30 days after\n");
	fprintf(fp, "%s", "Your receipt of the notice.\n\n");

	fprintf(fp, "%s", "5.2. If You initiate litigation against any entity by asserting a patent\n");
	fprintf(fp, "%s", "infringement claim (excluding declaratory judgment actions,\n");
	fprintf(fp, "%s", "counter-claims, and cross-claims) alleging that a Contributor Version\n");
	fprintf(fp, "%s", "directly or indirectly infringes any patent, then the rights granted to\n");
	fprintf(fp, "%s", "You by any and all Contributors for the Covered Software under Section\n");
	fprintf(fp, "%s", "2.1 of this License shall terminate.\n\n");

	fprintf(fp, "%s", "5.3. In the event of termination under Sections 5.1 or 5.2 above, all\n");
	fprintf(fp, "%s", "end user license agreements (excluding distributors and resellers) which\n");
	fprintf(fp, "%s", "have been validly granted by You or Your distributors under this License\n");
	fprintf(fp, "%s", "prior to termination shall survive termination.\n\n");

	fprintf(fp, "%s", "************************************************************************\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "*  6. Disclaimer of Warranty                                           *\n");
	fprintf(fp, "%s", "*  -------------------------                                           *\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "*  Covered Software is provided under this License on an \"as is\"       *\n");
	fprintf(fp, "%s", "*  basis, without warranty of any kind, either expressed, implied, or  *\n");
	fprintf(fp, "%s", "*  statutory, including, without limitation, warranties that the       *\n");
	fprintf(fp, "%s", "*  Covered Software is free of defects, merchantable, fit for a        *\n");
	fprintf(fp, "%s", "*  particular purpose or non-infringing. The entire risk as to the     *\n");
	fprintf(fp, "%s", "*  quality and performance of the Covered Software is with You.        *\n");
	fprintf(fp, "%s", "*  Should any Covered Software prove defective in any respect, You     *\n");
	fprintf(fp, "%s", "*  (not any Contributor) assume the cost of any necessary servicing,   *\n");
	fprintf(fp, "%s", "*  repair, or correction. This disclaimer of warranty constitutes an   *\n");
	fprintf(fp, "%s", "*  essential part of this License. No use of any Covered Software is   *\n");
	fprintf(fp, "%s", "*  authorized under this License except under this disclaimer.         *\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "************************************************************************\n\n");

	fprintf(fp, "%s", "************************************************************************\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "*  7. Limitation of Liability                                          *\n");
	fprintf(fp, "%s", "*  --------------------------                                          *\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "*  Under no circumstances and under no legal theory, whether tort      *\n");
	fprintf(fp, "%s", "*  (including negligence), contract, or otherwise, shall any           *\n");
	fprintf(fp, "%s", "*  Contributor, or anyone who distributes Covered Software as          *\n");
	fprintf(fp, "%s", "*  permitted above, be liable to You for any direct, indirect,         *\n");
	fprintf(fp, "%s", "*  special, incidental, or consequential damages of any character      *\n");
	fprintf(fp, "%s", "*  including, without limitation, damages for lost profits, loss of    *\n");
	fprintf(fp, "%s", "*  goodwill, work stoppage, computer failure or malfunction, or any    *\n");
	fprintf(fp, "%s", "*  and all other commercial damages or losses, even if such party      *\n");
	fprintf(fp, "%s", "*  shall have been informed of the possibility of such damages. This   *\n");
	fprintf(fp, "%s", "*  limitation of liability shall not apply to liability for death or   *\n");
	fprintf(fp, "%s", "*  personal injury resulting from such party's negligence to the       *\n");
	fprintf(fp, "%s", "*  extent applicable law prohibits such limitation. Some               *\n");
	fprintf(fp, "%s", "*  jurisdictions do not allow the exclusion or limitation of           *\n");
	fprintf(fp, "%s", "*  incidental or consequential damages, so this exclusion and          *\n");
	fprintf(fp, "%s", "*  limitation may not apply to You.                                    *\n");
	fprintf(fp, "%s", "*                                                                      *\n");
	fprintf(fp, "%s", "************************************************************************\n\n");

	fprintf(fp, "%s", "8. Litigation\n");
	fprintf(fp, "%s", "-------------\n\n");

	fprintf(fp, "%s", "Any litigation relating to this License may be brought only in the\n");
	fprintf(fp, "%s", "courts of a jurisdiction where the defendant maintains its principal\n");
	fprintf(fp, "%s", "place of business and such litigation shall be governed by laws of that\n");
	fprintf(fp, "%s", "jurisdiction, without reference to its conflict-of-law provisions.\n");
	fprintf(fp, "%s", "Nothing in this Section shall prevent a party's ability to bring\n");
	fprintf(fp, "%s", "cross-claims or counter-claims.\n\n");

	fprintf(fp, "%s", "9. Miscellaneous\n");
	fprintf(fp, "%s", "----------------\n\n");

	fprintf(fp, "%s", "This License represents the complete agreement concerning the subject\n");
	fprintf(fp, "%s", "matter hereof. If any provision of this License is held to be\n");
	fprintf(fp, "%s", "unenforceable, such provision shall be reformed only to the extent\n");
	fprintf(fp, "%s", "necessary to make it enforceable. Any law or regulation which provides\n");
	fprintf(fp, "%s", "that the language of a contract shall be construed against the drafter\n");
	fprintf(fp, "%s", "shall not be used to construe this License against a Contributor.\n\n");

	fprintf(fp, "%s", "10. Versions of the License\n");
	fprintf(fp, "%s", "---------------------------\n\n");

	fprintf(fp, "%s", "10.1. New Versions\n\n");

	fprintf(fp, "%s", "Mozilla Foundation is the license steward. Except as provided in Section\n");
	fprintf(fp, "%s", "10.3, no one other than the license steward has the right to modify or\n");
	fprintf(fp, "%s", "publish new versions of this License. Each version will be given a\n");
	fprintf(fp, "%s", "distinguishing version number.\n\n");

	fprintf(fp, "%s", "10.2. Effect of New Versions\n\n");

	fprintf(fp, "%s", "You may distribute the Covered Software under the terms of the version\n");
	fprintf(fp, "%s", "of the License under which You originally received the Covered Software,\n");
	fprintf(fp, "%s", "or under the terms of any subsequent version published by the license\n");
	fprintf(fp, "%s", "steward.\n\n");

	fprintf(fp, "%s", "10.3. Modified Versions\n\n");

	fprintf(fp, "%s", "If you create software not governed by this License, and you want to\n");
	fprintf(fp, "%s", "create a new license for such software, you may create and use a\n");
	fprintf(fp, "%s", "modified version of this License if you rename the license and remove\n");
	fprintf(fp, "%s", "any references to the name of the license steward (except to note that\n");
	fprintf(fp, "%s", "such modified license differs from this License).\n\n");

	fprintf(fp, "%s", "10.4. Distributing Source Code Form that is Incompatible With Secondary\n");
	fprintf(fp, "%s", "Licenses\n\n");

	fprintf(fp, "%s", "If You choose to distribute Source Code Form that is Incompatible With\n");
	fprintf(fp, "%s", "Secondary Licenses under the terms of this version of the License, the\n");
	fprintf(fp, "%s", "notice described in Exhibit B of this License must be attached.\n\n");

	fprintf(fp, "%s", "Exhibit A - Source Code Form License Notice\n");
	fprintf(fp, "%s", "-------------------------------------------\n\n");

	fprintf(fp, "%s", "  This Source Code Form is subject to the terms of the Mozilla Public\n");
	fprintf(fp, "%s", "  License, v. 2.0. If a copy of the MPL was not distributed with this\n");
	fprintf(fp, "%s", "  file, You can obtain one at http://mozilla.org/MPL/2.0/.\n\n");

	fprintf(fp, "%s", "If it is not possible or desirable to put the notice in a particular\n");
	fprintf(fp, "%s", "file, then You may include the notice in a location (such as a LICENSE\n");
	fprintf(fp, "%s", "file in a relevant directory) where a recipient would be likely to look\n");
	fprintf(fp, "%s", "for such a notice.\n\n");

	fprintf(fp, "%s", "You may add additional accurate notices of copyright ownership.\n\n");

	fprintf(fp, "%s", "Exhibit B - \"Incompatible With Secondary Licenses\" Notice\n");
	fprintf(fp, "%s", "---------------------------------------------------------\n\n");

	fprintf(fp, "%s", "  This Source Code Form is \"Incompatible With Secondary Licenses\", as\n");
	fprintf(fp, "%s", "  defined by the Mozilla Public License, v. 2.0.\n");

	fclose(fp);
}

/* saves a LICENSE file for GPL2 */
static void save_license_gpl2(char * filename)
{
	FILE * fp;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"                    GNU GENERAL PUBLIC LICENSE\n");
	fprintf(fp,"                       Version 2, June 1991\n\n");

	fprintf(fp," Copyright (C) 1989, 1991 Free Software Foundation, Inc.,\n");
	fprintf(fp," 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n");
	fprintf(fp," Everyone is permitted to copy and distribute verbatim copies\n");
	fprintf(fp," of this license document, but changing it is not allowed.\n\n");

	fprintf(fp,"                            Preamble\n\n");

	fprintf(fp,"  The licenses for most software are designed to take away your\n");
	fprintf(fp,"freedom to share and change it.  By contrast, the GNU General Public\n");
	fprintf(fp,"License is intended to guarantee your freedom to share and change free\n");
	fprintf(fp,"software--to make sure the software is free for all its users.  This\n");
	fprintf(fp,"General Public License applies to most of the Free Software\n");
	fprintf(fp,"Foundation's software and to any other program whose authors commit to\n");
	fprintf(fp,"using it.  (Some other Free Software Foundation software is covered by\n");
	fprintf(fp,"the GNU Lesser General Public License instead.)  You can apply it to\n");
	fprintf(fp,"your programs, too.\n\n");

	fprintf(fp,"  When we speak of free software, we are referring to freedom, not\n");
	fprintf(fp,"price.  Our General Public Licenses are designed to make sure that you\n");
	fprintf(fp,"have the freedom to distribute copies of free software (and charge for\n");
	fprintf(fp,"this service if you wish), that you receive source code or can get it\n");
	fprintf(fp,"if you want it, that you can change the software or use pieces of it\n");
	fprintf(fp,"in new free programs; and that you know you can do these things.\n\n");

	fprintf(fp,"  To protect your rights, we need to make restrictions that forbid\n");
	fprintf(fp,"anyone to deny you these rights or to ask you to surrender the rights.\n");
	fprintf(fp,"These restrictions translate to certain responsibilities for you if you\n");
	fprintf(fp,"distribute copies of the software, or if you modify it.\n\n");

	fprintf(fp,"  For example, if you distribute copies of such a program, whether\n");
	fprintf(fp,"gratis or for a fee, you must give the recipients all the rights that\n");
	fprintf(fp,"you have.  You must make sure that they, too, receive or can get the\n");
	fprintf(fp,"source code.  And you must show them these terms so they know their\n");
	fprintf(fp,"rights.\n\n");

	fprintf(fp,"  We protect your rights with two steps: (1) copyright the software, and\n");
	fprintf(fp,"(2) offer you this license which gives you legal permission to copy,\n");
	fprintf(fp,"distribute and/or modify the software.\n\n");

	fprintf(fp,"  Also, for each author's protection and ours, we want to make certain\n");
	fprintf(fp,"that everyone understands that there is no warranty for this free\n");
	fprintf(fp,"software.  If the software is modified by someone else and passed on, we\n");
	fprintf(fp,"want its recipients to know that what they have is not the original, so\n");
	fprintf(fp,"that any problems introduced by others will not reflect on the original\n");
	fprintf(fp,"authors' reputations.\n\n");

	fprintf(fp,"  Finally, any free program is threatened constantly by software\n");
	fprintf(fp,"patents.  We wish to avoid the danger that redistributors of a free\n");
	fprintf(fp,"program will individually obtain patent licenses, in effect making the\n");
	fprintf(fp,"program proprietary.  To prevent this, we have made it clear that any\n");
	fprintf(fp,"patent must be licensed for everyone's free use or not licensed at all.\n\n");

	fprintf(fp,"  The precise terms and conditions for copying, distribution and\n");
	fprintf(fp,"modification follow.\n\n");

	fprintf(fp,"                    GNU GENERAL PUBLIC LICENSE\n");
	fprintf(fp,"   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n\n");

	fprintf(fp,"  0. This License applies to any program or other work which contains\n");
	fprintf(fp,"a notice placed by the copyright holder saying it may be distributed\n");
	fprintf(fp,"under the terms of this General Public License.  The \"Program\", below,\n");
	fprintf(fp,"refers to any such program or work, and a \"work based on the Program\"\n");
	fprintf(fp,"means either the Program or any derivative work under copyright law:\n");
	fprintf(fp,"that is to say, a work containing the Program or a portion of it,\n");
	fprintf(fp,"either verbatim or with modifications and/or translated into another\n");
	fprintf(fp,"language.  (Hereinafter, translation is included without limitation in\n");
	fprintf(fp,"the term \"modification\".)  Each licensee is addressed as \"you\".\n\n");

	fprintf(fp,"Activities other than copying, distribution and modification are not\n");
	fprintf(fp,"covered by this License; they are outside its scope.  The act of\n");
	fprintf(fp,"running the Program is not restricted, and the output from the Program\n");
	fprintf(fp,"is covered only if its contents constitute a work based on the\n");
	fprintf(fp,"Program (independent of having been made by running the Program).\n");
	fprintf(fp,"Whether that is true depends on what the Program does.\n\n");

	fprintf(fp,"  1. You may copy and distribute verbatim copies of the Program's\n");
	fprintf(fp,"source code as you receive it, in any medium, provided that you\n");
	fprintf(fp,"conspicuously and appropriately publish on each copy an appropriate\n");
	fprintf(fp,"copyright notice and disclaimer of warranty; keep intact all the\n");
	fprintf(fp,"notices that refer to this License and to the absence of any warranty;\n");
	fprintf(fp,"and give any other recipients of the Program a copy of this License\n");
	fprintf(fp,"along with the Program.\n\n");

	fprintf(fp,"You may charge a fee for the physical act of transferring a copy, and\n");
	fprintf(fp,"you may at your option offer warranty protection in exchange for a fee.\n\n");

	fprintf(fp,"  2. You may modify your copy or copies of the Program or any portion\n");
	fprintf(fp,"of it, thus forming a work based on the Program, and copy and\n");
	fprintf(fp,"distribute such modifications or work under the terms of Section 1\n");
	fprintf(fp,"above, provided that you also meet all of these conditions:\n\n");

	fprintf(fp,"    a) You must cause the modified files to carry prominent notices\n");
	fprintf(fp,"    stating that you changed the files and the date of any change.\n\n");

	fprintf(fp,"    b) You must cause any work that you distribute or publish, that in\n");
	fprintf(fp,"    whole or in part contains or is derived from the Program or any\n");
	fprintf(fp,"    part thereof, to be licensed as a whole at no charge to all third\n");
	fprintf(fp,"    parties under the terms of this License.\n\n");

	fprintf(fp,"    c) If the modified program normally reads commands interactively\n");
	fprintf(fp,"    when run, you must cause it, when started running for such\n");
	fprintf(fp,"    interactive use in the most ordinary way, to print or display an\n");
	fprintf(fp,"    announcement including an appropriate copyright notice and a\n");
	fprintf(fp,"    notice that there is no warranty (or else, saying that you provide\n");
	fprintf(fp,"    a warranty) and that users may redistribute the program under\n");
	fprintf(fp,"    these conditions, and telling the user how to view a copy of this\n");
	fprintf(fp,"    License.  (Exception: if the Program itself is interactive but\n");
	fprintf(fp,"    does not normally print such an announcement, your work based on\n");
	fprintf(fp,"    the Program is not required to print an announcement.)\n\n");

	fprintf(fp,"These requirements apply to the modified work as a whole.  If\n");
	fprintf(fp,"identifiable sections of that work are not derived from the Program,\n");
	fprintf(fp,"and can be reasonably considered independent and separate works in\n");
	fprintf(fp,"themselves, then this License, and its terms, do not apply to those\n");
	fprintf(fp,"sections when you distribute them as separate works.  But when you\n");
	fprintf(fp,"distribute the same sections as part of a whole which is a work based\n");
	fprintf(fp,"on the Program, the distribution of the whole must be on the terms of\n");
	fprintf(fp,"this License, whose permissions for other licensees extend to the\n");
	fprintf(fp,"entire whole, and thus to each and every part regardless of who wrote it.\n\n");

	fprintf(fp,"Thus, it is not the intent of this section to claim rights or contest\n");
	fprintf(fp,"your rights to work written entirely by you; rather, the intent is to\n");
	fprintf(fp,"exercise the right to control the distribution of derivative or\n");
	fprintf(fp,"collective works based on the Program.\n\n");

	fprintf(fp,"In addition, mere aggregation of another work not based on the Program\n");
	fprintf(fp,"with the Program (or with a work based on the Program) on a volume of\n");
	fprintf(fp,"a storage or distribution medium does not bring the other work under\n");
	fprintf(fp,"the scope of this License.\n\n");

	fprintf(fp,"  3. You may copy and distribute the Program (or a work based on it,\n");
	fprintf(fp,"under Section 2) in object code or executable form under the terms of\n");
	fprintf(fp,"Sections 1 and 2 above provided that you also do one of the following:\n\n");

	fprintf(fp,"    a) Accompany it with the complete corresponding machine-readable\n");
	fprintf(fp,"    source code, which must be distributed under the terms of Sections\n");
	fprintf(fp,"    1 and 2 above on a medium customarily used for software interchange; or,\n\n");

	fprintf(fp,"    b) Accompany it with a written offer, valid for at least three\n");
	fprintf(fp,"    years, to give any third party, for a charge no more than your\n");
	fprintf(fp,"    cost of physically performing source distribution, a complete\n");
	fprintf(fp,"    machine-readable copy of the corresponding source code, to be\n");
	fprintf(fp,"    distributed under the terms of Sections 1 and 2 above on a medium\n");
	fprintf(fp,"    customarily used for software interchange; or,\n\n");

	fprintf(fp,"    c) Accompany it with the information you received as to the offer\n");
	fprintf(fp,"    to distribute corresponding source code.  (This alternative is\n");
	fprintf(fp,"    allowed only for noncommercial distribution and only if you\n");
	fprintf(fp,"    received the program in object code or executable form with such\n");
	fprintf(fp,"    an offer, in accord with Subsection b above.)\n\n");

	fprintf(fp,"The source code for a work means the preferred form of the work for\n");
	fprintf(fp,"making modifications to it.  For an executable work, complete source\n");
	fprintf(fp,"code means all the source code for all modules it contains, plus any\n");
	fprintf(fp,"associated interface definition files, plus the scripts used to\n");
	fprintf(fp,"control compilation and installation of the executable.  However, as a\n");
	fprintf(fp,"special exception, the source code distributed need not include\n");
	fprintf(fp,"anything that is normally distributed (in either source or binary\n");
	fprintf(fp,"form) with the major components (compiler, kernel, and so on) of the\n");
	fprintf(fp,"operating system on which the executable runs, unless that component\n");
	fprintf(fp,"itself accompanies the executable.\n\n");

	fprintf(fp,"If distribution of executable or object code is made by offering\n");
	fprintf(fp,"access to copy from a designated place, then offering equivalent\n");
	fprintf(fp,"access to copy the source code from the same place counts as\n");
	fprintf(fp,"distribution of the source code, even though third parties are not\n");
	fprintf(fp,"compelled to copy the source along with the object code.\n\n");

	fprintf(fp,"  4. You may not copy, modify, sublicense, or distribute the Program\n");
	fprintf(fp,"except as expressly provided under this License.  Any attempt\n");
	fprintf(fp,"otherwise to copy, modify, sublicense or distribute the Program is\n");
	fprintf(fp,"void, and will automatically terminate your rights under this License.\n");
	fprintf(fp,"However, parties who have received copies, or rights, from you under\n");
	fprintf(fp,"this License will not have their licenses terminated so long as such\n");
	fprintf(fp,"parties remain in full compliance.\n\n");

	fprintf(fp,"  5. You are not required to accept this License, since you have not\n");
	fprintf(fp,"signed it.  However, nothing else grants you permission to modify or\n");
	fprintf(fp,"distribute the Program or its derivative works.  These actions are\n");
	fprintf(fp,"prohibited by law if you do not accept this License.  Therefore, by\n");
	fprintf(fp,"modifying or distributing the Program (or any work based on the\n");
	fprintf(fp,"Program), you indicate your acceptance of this License to do so, and\n");
	fprintf(fp,"all its terms and conditions for copying, distributing or modifying\n");
	fprintf(fp,"the Program or works based on it.\n\n");

	fprintf(fp,"  6. Each time you redistribute the Program (or any work based on the\n");
	fprintf(fp,"Program), the recipient automatically receives a license from the\n");
	fprintf(fp,"original licensor to copy, distribute or modify the Program subject to\n");
	fprintf(fp,"these terms and conditions.  You may not impose any further\n");
	fprintf(fp,"restrictions on the recipients' exercise of the rights granted herein.\n");
	fprintf(fp,"You are not responsible for enforcing compliance by third parties to\n");
	fprintf(fp,"this License.\n\n");

	fprintf(fp,"  7. If, as a consequence of a court judgment or allegation of patent\n");
	fprintf(fp,"infringement or for any other reason (not limited to patent issues),\n");
	fprintf(fp,"conditions are imposed on you (whether by court order, agreement or\n");
	fprintf(fp,"otherwise) that contradict the conditions of this License, they do not\n");
	fprintf(fp,"excuse you from the conditions of this License.  If you cannot\n");
	fprintf(fp,"distribute so as to satisfy simultaneously your obligations under this\n");
	fprintf(fp,"License and any other pertinent obligations, then as a consequence you\n");
	fprintf(fp,"may not distribute the Program at all.  For example, if a patent\n");
	fprintf(fp,"license would not permit royalty-free redistribution of the Program by\n");
	fprintf(fp,"all those who receive copies directly or indirectly through you, then\n");
	fprintf(fp,"the only way you could satisfy both it and this License would be to\n");
	fprintf(fp,"refrain entirely from distribution of the Program.\n\n");

	fprintf(fp,"If any portion of this section is held invalid or unenforceable under\n");
	fprintf(fp,"any particular circumstance, the balance of the section is intended to\n");
	fprintf(fp,"apply and the section as a whole is intended to apply in other\n");
	fprintf(fp,"circumstances.\n\n");

	fprintf(fp,"It is not the purpose of this section to induce you to infringe any\n");
	fprintf(fp,"patents or other property right claims or to contest validity of any\n");
	fprintf(fp,"such claims; this section has the sole purpose of protecting the\n");
	fprintf(fp,"integrity of the free software distribution system, which is\n");
	fprintf(fp,"implemented by public license practices.  Many people have made\n");
	fprintf(fp,"generous contributions to the wide range of software distributed\n");
	fprintf(fp,"through that system in reliance on consistent application of that\n");
	fprintf(fp,"system; it is up to the author/donor to decide if he or she is willing\n");
	fprintf(fp,"to distribute software through any other system and a licensee cannot\n");
	fprintf(fp,"impose that choice.\n\n");

	fprintf(fp,"This section is intended to make thoroughly clear what is believed to\n");
	fprintf(fp,"be a consequence of the rest of this License.\n\n");

	fprintf(fp,"  8. If the distribution and/or use of the Program is restricted in\n");
	fprintf(fp,"certain countries either by patents or by copyrighted interfaces, the\n");
	fprintf(fp,"original copyright holder who places the Program under this License\n");
	fprintf(fp,"may add an explicit geographical distribution limitation excluding\n");
	fprintf(fp,"those countries, so that distribution is permitted only in or among\n");
	fprintf(fp,"countries not thus excluded.  In such case, this License incorporates\n");
	fprintf(fp,"the limitation as if written in the body of this License.\n\n");

	fprintf(fp,"  9. The Free Software Foundation may publish revised and/or new versions\n");
	fprintf(fp,"of the General Public License from time to time.  Such new versions will\n");
	fprintf(fp,"be similar in spirit to the present version, but may differ in detail to\n");
	fprintf(fp,"address new problems or concerns.\n\n");

	fprintf(fp,"Each version is given a distinguishing version number.  If the Program\n");
	fprintf(fp,"specifies a version number of this License which applies to it and \"any\n");
	fprintf(fp,"later version\", you have the option of following the terms and conditions\n");
	fprintf(fp,"either of that version or of any later version published by the Free\n");
	fprintf(fp,"Software Foundation.  If the Program does not specify a version number of\n");
	fprintf(fp,"this License, you may choose any version ever published by the Free Software\n");
	fprintf(fp,"Foundation.\n\n");

	fprintf(fp,"  10. If you wish to incorporate parts of the Program into other free\n");
	fprintf(fp,"programs whose distribution conditions are different, write to the author\n");
	fprintf(fp,"to ask for permission.  For software which is copyrighted by the Free\n");
	fprintf(fp,"Software Foundation, write to the Free Software Foundation; we sometimes\n");
	fprintf(fp,"make exceptions for this.  Our decision will be guided by the two goals\n");
	fprintf(fp,"of preserving the free status of all derivatives of our free software and\n");
	fprintf(fp,"of promoting the sharing and reuse of software generally.\n\n");

	fprintf(fp,"                            NO WARRANTY\n\n");

	fprintf(fp,"  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\n");
	fprintf(fp,"FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\n");
	fprintf(fp,"OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\n");
	fprintf(fp,"PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\n");
	fprintf(fp,"OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n");
	fprintf(fp,"MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\n");
	fprintf(fp,"TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\n");
	fprintf(fp,"PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\n");
	fprintf(fp,"REPAIR OR CORRECTION.\n\n");

	fprintf(fp,"  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n");
	fprintf(fp,"WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\n");
	fprintf(fp,"REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\n");
	fprintf(fp,"INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\n");
	fprintf(fp,"OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\n");
	fprintf(fp,"TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\n");
	fprintf(fp,"YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\n");
	fprintf(fp,"PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\n");
	fprintf(fp,"POSSIBILITY OF SUCH DAMAGES.\n");

	fclose(fp);
}

/* saves a LICENSE file for GPL3 */
static void save_license_gpl3(char * filename)
{
	FILE * fp;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"                    GNU GENERAL PUBLIC LICENSE\n");
	fprintf(fp,"                       Version 3, 29 June 2007\n\n");

	fprintf(fp," Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>\n");
	fprintf(fp," Everyone is permitted to copy and distribute verbatim copies\n");
	fprintf(fp," of this license document, but changing it is not allowed.\n\n");

	fprintf(fp,"                            Preamble\n\n");

	fprintf(fp,"  The GNU General Public License is a free, copyleft license for\n");
	fprintf(fp,"software and other kinds of works.\n\n");

	fprintf(fp,"  The licenses for most software and other practical works are designed\n");
	fprintf(fp,"to take away your freedom to share and change the works.  By contrast,\n");
	fprintf(fp,"the GNU General Public License is intended to guarantee your freedom to\n");
	fprintf(fp,"share and change all versions of a program--to make sure it remains free\n");
	fprintf(fp,"software for all its users.  We, the Free Software Foundation, use the\n");
	fprintf(fp,"GNU General Public License for most of our software; it applies also to\n");
	fprintf(fp,"any other work released this way by its authors.  You can apply it to\n");
	fprintf(fp,"your programs, too.\n\n");

	fprintf(fp,"  When we speak of free software, we are referring to freedom, not\n");
	fprintf(fp,"price.  Our General Public Licenses are designed to make sure that you\n");
	fprintf(fp,"have the freedom to distribute copies of free software (and charge for\n");
	fprintf(fp,"them if you wish), that you receive source code or can get it if you\n");
	fprintf(fp,"want it, that you can change the software or use pieces of it in new\n");
	fprintf(fp,"free programs, and that you know you can do these things.\n\n");

	fprintf(fp,"  To protect your rights, we need to prevent others from denying you\n");
	fprintf(fp,"these rights or asking you to surrender the rights.  Therefore, you have\n");
	fprintf(fp,"certain responsibilities if you distribute copies of the software, or if\n");
	fprintf(fp,"you modify it: responsibilities to respect the freedom of others.\n\n");

	fprintf(fp,"  For example, if you distribute copies of such a program, whether\n");
	fprintf(fp,"gratis or for a fee, you must pass on to the recipients the same\n");
	fprintf(fp,"freedoms that you received.  You must make sure that they, too, receive\n");
	fprintf(fp,"or can get the source code.  And you must show them these terms so they\n");
	fprintf(fp,"know their rights.\n\n");

	fprintf(fp,"  Developers that use the GNU GPL protect your rights with two steps:\n");
	fprintf(fp,"(1) assert copyright on the software, and (2) offer you this License\n");
	fprintf(fp,"giving you legal permission to copy, distribute and/or modify it.\n\n");

	fprintf(fp,"  For the developers' and authors' protection, the GPL clearly explains\n");
	fprintf(fp,"that there is no warranty for this free software.  For both users' and\n");
	fprintf(fp,"authors' sake, the GPL requires that modified versions be marked as\n");
	fprintf(fp,"changed, so that their problems will not be attributed erroneously to\n");
	fprintf(fp,"authors of previous versions.\n\n");

	fprintf(fp,"  Some devices are designed to deny users access to install or run\n");
	fprintf(fp,"modified versions of the software inside them, although the manufacturer\n");
	fprintf(fp,"can do so.  This is fundamentally incompatible with the aim of\n");
	fprintf(fp,"protecting users' freedom to change the software.  The systematic\n");
	fprintf(fp,"pattern of such abuse occurs in the area of products for individuals to\n");
	fprintf(fp,"use, which is precisely where it is most unacceptable.  Therefore, we\n");
	fprintf(fp,"have designed this version of the GPL to prohibit the practice for those\n");
	fprintf(fp,"products.  If such problems arise substantially in other domains, we\n");
	fprintf(fp,"stand ready to extend this provision to those domains in future versions\n");
	fprintf(fp,"of the GPL, as needed to protect the freedom of users.\n\n");

	fprintf(fp,"  Finally, every program is threatened constantly by software patents.\n");
	fprintf(fp,"States should not allow patents to restrict development and use of\n");
	fprintf(fp,"software on general-purpose computers, but in those that do, we wish to\n");
	fprintf(fp,"avoid the special danger that patents applied to a free program could\n");
	fprintf(fp,"make it effectively proprietary.  To prevent this, the GPL assures that\n");
	fprintf(fp,"patents cannot be used to render the program non-free.\n\n");

	fprintf(fp,"  The precise terms and conditions for copying, distribution and\n");
	fprintf(fp,"modification follow.\n\n");

	fprintf(fp,"                       TERMS AND CONDITIONS\n\n");

	fprintf(fp,"  0. Definitions.\n\n");

	fprintf(fp,"  \"This License\" refers to version 3 of the GNU General Public License.\n\n");

	fprintf(fp,"  \"Copyright\" also means copyright-like laws that apply to other kinds of\n");
	fprintf(fp,"works, such as semiconductor masks.\n\n");

	fprintf(fp,"  \"The Program\" refers to any copyrightable work licensed under this\n");
	fprintf(fp,"License.  Each licensee is addressed as \"you\".  \"Licensees\" and\n");
	fprintf(fp,"\"recipients\" may be individuals or organizations.\n\n");

	fprintf(fp,"  To \"modify\" a work means to copy from or adapt all or part of the work\n");
	fprintf(fp,"in a fashion requiring copyright permission, other than the making of an\n");
	fprintf(fp,"exact copy.  The resulting work is called a \"modified version\" of the\n");
	fprintf(fp,"earlier work or a work \"based on\" the earlier work.\n\n");

	fprintf(fp,"  A \"covered work\" means either the unmodified Program or a work based\n");
	fprintf(fp,"on the Program.\n\n");

	fprintf(fp,"  To \"propagate\" a work means to do anything with it that, without\n");
	fprintf(fp,"permission, would make you directly or secondarily liable for\n");
	fprintf(fp,"infringement under applicable copyright law, except executing it on a\n");
	fprintf(fp,"computer or modifying a private copy.  Propagation includes copying,\n");
	fprintf(fp,"distribution (with or without modification), making available to the\n");
	fprintf(fp,"public, and in some countries other activities as well.\n\n");

	fprintf(fp,"  To \"convey\" a work means any kind of propagation that enables other\n");
	fprintf(fp,"parties to make or receive copies.  Mere interaction with a user through\n");
	fprintf(fp,"a computer network, with no transfer of a copy, is not conveying.\n\n");

	fprintf(fp,"  An interactive user interface displays \"Appropriate Legal Notices\"\n");
	fprintf(fp,"to the extent that it includes a convenient and prominently visible\n");
	fprintf(fp,"feature that (1) displays an appropriate copyright notice, and (2)\n");
	fprintf(fp,"tells the user that there is no warranty for the work (except to the\n");
	fprintf(fp,"extent that warranties are provided), that licensees may convey the\n");
	fprintf(fp,"work under this License, and how to view a copy of this License.  If\n");
	fprintf(fp,"the interface presents a list of user commands or options, such as a\n");
	fprintf(fp,"menu, a prominent item in the list meets this criterion.\n\n");

	fprintf(fp,"  1. Source Code.\n\n");

	fprintf(fp,"  The \"source code\" for a work means the preferred form of the work\n");
	fprintf(fp,"for making modifications to it.  \"Object code\" means any non-source\n");
	fprintf(fp,"form of a work.\n\n");

	fprintf(fp,"  A \"Standard Interface\" means an interface that either is an official\n");
	fprintf(fp,"standard defined by a recognized standards body, or, in the case of\n");
	fprintf(fp,"interfaces specified for a particular programming language, one that\n");
	fprintf(fp,"is widely used among developers working in that language.\n\n");

	fprintf(fp,"  The \"System Libraries\" of an executable work include anything, other\n");
	fprintf(fp,"than the work as a whole, that (a) is included in the normal form of\n");
	fprintf(fp,"packaging a Major Component, but which is not part of that Major\n");
	fprintf(fp,"Component, and (b) serves only to enable use of the work with that\n");
	fprintf(fp,"Major Component, or to implement a Standard Interface for which an\n");
	fprintf(fp,"implementation is available to the public in source code form.  A\n");
	fprintf(fp,"\"Major Component\", in this context, means a major essential component\n");
	fprintf(fp,"(kernel, window system, and so on) of the specific operating system\n");
	fprintf(fp,"(if any) on which the executable work runs, or a compiler used to\n");
	fprintf(fp,"produce the work, or an object code interpreter used to run it.\n\n");

	fprintf(fp,"  The \"Corresponding Source\" for a work in object code form means all\n");
	fprintf(fp,"the source code needed to generate, install, and (for an executable\n");
	fprintf(fp,"work) run the object code and to modify the work, including scripts to\n");
	fprintf(fp,"control those activities.  However, it does not include the work's\n");
	fprintf(fp,"System Libraries, or general-purpose tools or generally available free\n");
	fprintf(fp,"programs which are used unmodified in performing those activities but\n");
	fprintf(fp,"which are not part of the work.  For example, Corresponding Source\n");
	fprintf(fp,"includes interface definition files associated with source files for\n");
	fprintf(fp,"the work, and the source code for shared libraries and dynamically\n");
	fprintf(fp,"linked subprograms that the work is specifically designed to require,\n");
	fprintf(fp,"such as by intimate data communication or control flow between those\n");
	fprintf(fp,"subprograms and other parts of the work.\n\n");

	fprintf(fp,"  The Corresponding Source need not include anything that users\n");
	fprintf(fp,"can regenerate automatically from other parts of the Corresponding\n");
	fprintf(fp,"Source.\n\n");

	fprintf(fp,"  The Corresponding Source for a work in source code form is that\n");
	fprintf(fp,"same work.\n\n");

	fprintf(fp,"  2. Basic Permissions.\n\n");

	fprintf(fp,"  All rights granted under this License are granted for the term of\n");
	fprintf(fp,"copyright on the Program, and are irrevocable provided the stated\n");
	fprintf(fp,"conditions are met.  This License explicitly affirms your unlimited\n");
	fprintf(fp,"permission to run the unmodified Program.  The output from running a\n");
	fprintf(fp,"covered work is covered by this License only if the output, given its\n");
	fprintf(fp,"content, constitutes a covered work.  This License acknowledges your\n");
	fprintf(fp,"rights of fair use or other equivalent, as provided by copyright law.\n\n");

	fprintf(fp,"  You may make, run and propagate covered works that you do not\n");
	fprintf(fp,"convey, without conditions so long as your license otherwise remains\n");
	fprintf(fp,"in force.  You may convey covered works to others for the sole purpose\n");
	fprintf(fp,"of having them make modifications exclusively for you, or provide you\n");
	fprintf(fp,"with facilities for running those works, provided that you comply with\n");
	fprintf(fp,"the terms of this License in conveying all material for which you do\n");
	fprintf(fp,"not control copyright.  Those thus making or running the covered works\n");
	fprintf(fp,"for you must do so exclusively on your behalf, under your direction\n");
	fprintf(fp,"and control, on terms that prohibit them from making any copies of\n");
	fprintf(fp,"your copyrighted material outside their relationship with you.\n\n");

	fprintf(fp,"  Conveying under any other circumstances is permitted solely under\n");
	fprintf(fp,"the conditions stated below.  Sublicensing is not allowed; section 10\n");
	fprintf(fp,"makes it unnecessary.\n\n");

	fprintf(fp,"  3. Protecting Users' Legal Rights From Anti-Circumvention Law.\n\n");

	fprintf(fp,"  No covered work shall be deemed part of an effective technological\n");
	fprintf(fp,"measure under any applicable law fulfilling obligations under article\n");
	fprintf(fp,"11 of the WIPO copyright treaty adopted on 20 December 1996, or\n");
	fprintf(fp,"similar laws prohibiting or restricting circumvention of such\n");
	fprintf(fp,"measures.\n\n");

	fprintf(fp,"  When you convey a covered work, you waive any legal power to forbid\n");
	fprintf(fp,"circumvention of technological measures to the extent such circumvention\n");
	fprintf(fp,"is effected by exercising rights under this License with respect to\n");
	fprintf(fp,"the covered work, and you disclaim any intention to limit operation or\n");
	fprintf(fp,"modification of the work as a means of enforcing, against the work's\n");
	fprintf(fp,"users, your or third parties' legal rights to forbid circumvention of\n");
	fprintf(fp,"technological measures.\n\n");

	fprintf(fp,"  4. Conveying Verbatim Copies.\n\n");

	fprintf(fp,"  You may convey verbatim copies of the Program's source code as you\n");
	fprintf(fp,"receive it, in any medium, provided that you conspicuously and\n");
	fprintf(fp,"appropriately publish on each copy an appropriate copyright notice;\n");
	fprintf(fp,"keep intact all notices stating that this License and any\n");
	fprintf(fp,"non-permissive terms added in accord with section 7 apply to the code;\n");
	fprintf(fp,"keep intact all notices of the absence of any warranty; and give all\n");
	fprintf(fp,"recipients a copy of this License along with the Program.\n\n");

	fprintf(fp,"  You may charge any price or no price for each copy that you convey,\n");
	fprintf(fp,"and you may offer support or warranty protection for a fee.\n\n");

	fprintf(fp,"  5. Conveying Modified Source Versions.\n\n");

	fprintf(fp,"  You may convey a work based on the Program, or the modifications to\n");
	fprintf(fp,"produce it from the Program, in the form of source code under the\n");
	fprintf(fp,"terms of section 4, provided that you also meet all of these conditions:\n\n");

	fprintf(fp,"    a) The work must carry prominent notices stating that you modified\n");
	fprintf(fp,"    it, and giving a relevant date.\n\n");

	fprintf(fp,"    b) The work must carry prominent notices stating that it is\n");
	fprintf(fp,"    released under this License and any conditions added under section\n");
	fprintf(fp,"    7.  This requirement modifies the requirement in section 4 to\n");
	fprintf(fp,"    \"keep intact all notices\".\n\n");

	fprintf(fp,"    c) You must license the entire work, as a whole, under this\n");
	fprintf(fp,"    License to anyone who comes into possession of a copy.  This\n");
	fprintf(fp,"    License will therefore apply, along with any applicable section 7\n");
	fprintf(fp,"    additional terms, to the whole of the work, and all its parts,\n");
	fprintf(fp,"    regardless of how they are packaged.  This License gives no\n");
	fprintf(fp,"    permission to license the work in any other way, but it does not\n");
	fprintf(fp,"    invalidate such permission if you have separately received it.\n\n");

	fprintf(fp,"    d) If the work has interactive user interfaces, each must display\n");
	fprintf(fp,"    Appropriate Legal Notices; however, if the Program has interactive\n");
	fprintf(fp,"    interfaces that do not display Appropriate Legal Notices, your\n");
	fprintf(fp,"    work need not make them do so.\n\n");

	fprintf(fp,"  A compilation of a covered work with other separate and independent\n");
	fprintf(fp,"works, which are not by their nature extensions of the covered work,\n");
	fprintf(fp,"and which are not combined with it such as to form a larger program,\n");
	fprintf(fp,"in or on a volume of a storage or distribution medium, is called an\n");
	fprintf(fp,"\"aggregate\" if the compilation and its resulting copyright are not\n");
	fprintf(fp,"used to limit the access or legal rights of the compilation's users\n");
	fprintf(fp,"beyond what the individual works permit.  Inclusion of a covered work\n");
	fprintf(fp,"in an aggregate does not cause this License to apply to the other\n");
	fprintf(fp,"parts of the aggregate.\n\n");

	fprintf(fp,"  6. Conveying Non-Source Forms.\n\n");

	fprintf(fp,"  You may convey a covered work in object code form under the terms\n");
	fprintf(fp,"of sections 4 and 5, provided that you also convey the\n");
	fprintf(fp,"machine-readable Corresponding Source under the terms of this License,\n");
	fprintf(fp,"in one of these ways:\n\n");

	fprintf(fp,"    a) Convey the object code in, or embodied in, a physical product\n");
	fprintf(fp,"    (including a physical distribution medium), accompanied by the\n");
	fprintf(fp,"    Corresponding Source fixed on a durable physical medium\n");
	fprintf(fp,"    customarily used for software interchange.\n\n");

	fprintf(fp,"    b) Convey the object code in, or embodied in, a physical product\n");
	fprintf(fp,"    (including a physical distribution medium), accompanied by a\n");
	fprintf(fp,"    written offer, valid for at least three years and valid for as\n");
	fprintf(fp,"    long as you offer spare parts or customer support for that product\n");
	fprintf(fp,"    model, to give anyone who possesses the object code either (1) a\n");
	fprintf(fp,"    copy of the Corresponding Source for all the software in the\n");
	fprintf(fp,"    product that is covered by this License, on a durable physical\n");
	fprintf(fp,"    medium customarily used for software interchange, for a price no\n");
	fprintf(fp,"    more than your reasonable cost of physically performing this\n");
	fprintf(fp,"    conveying of source, or (2) access to copy the\n");
	fprintf(fp,"    Corresponding Source from a network server at no charge.\n\n");

	fprintf(fp,"	c) Convey individual copies of the object code with a copy of the\n");
	fprintf(fp,"    written offer to provide the Corresponding Source.  This\n");
	fprintf(fp,"    alternative is allowed only occasionally and noncommercially, and\n");
	fprintf(fp,"    only if you received the object code with such an offer, in accord\n");
	fprintf(fp,"    with subsection 6b.\n\n");

	fprintf(fp,"    d) Convey the object code by offering access from a designated\n");
	fprintf(fp,"    place (gratis or for a charge), and offer equivalent access to the\n");
	fprintf(fp,"    Corresponding Source in the same way through the same place at no\n");
	fprintf(fp,"    further charge.  You need not require recipients to copy the\n");
	fprintf(fp,"    Corresponding Source along with the object code.  If the place to\n");
	fprintf(fp,"    copy the object code is a network server, the Corresponding Source\n");
	fprintf(fp,"    may be on a different server (operated by you or a third party)\n");
	fprintf(fp,"    that supports equivalent copying facilities, provided you maintain\n");
	fprintf(fp,"    clear directions next to the object code saying where to find the\n");
	fprintf(fp,"    Corresponding Source.  Regardless of what server hosts the\n");
	fprintf(fp,"    Corresponding Source, you remain obligated to ensure that it is\n");
	fprintf(fp,"    available for as long as needed to satisfy these requirements.\n\n");

	fprintf(fp,"    e) Convey the object code using peer-to-peer transmission, provided\n");
	fprintf(fp,"    you inform other peers where the object code and Corresponding\n");
	fprintf(fp,"    Source of the work are being offered to the general public at no\n");
	fprintf(fp,"    charge under subsection 6d.\n\n");

	fprintf(fp,"  A separable portion of the object code, whose source code is excluded\n");
	fprintf(fp,"from the Corresponding Source as a System Library, need not be\n");
	fprintf(fp,"included in conveying the object code work.\n\n");

	fprintf(fp,"  A \"User Product\" is either (1) a \"consumer product\", which means any\n");
	fprintf(fp,"tangible personal property which is normally used for personal, family,\n");
	fprintf(fp,"or household purposes, or (2) anything designed or sold for incorporation\n");
	fprintf(fp,"into a dwelling.  In determining whether a product is a consumer product,\n");
	fprintf(fp,"doubtful cases shall be resolved in favor of coverage.  For a particular\n");
	fprintf(fp,"product received by a particular user, \"normally used\" refers to a\n");
	fprintf(fp,"typical or common use of that class of product, regardless of the status\n");
	fprintf(fp,"of the particular user or of the way in which the particular user\n");
	fprintf(fp,"actually uses, or expects or is expected to use, the product.  A product\n");
	fprintf(fp,"is a consumer product regardless of whether the product has substantial\n");
	fprintf(fp,"commercial, industrial or non-consumer uses, unless such uses represent\n");
	fprintf(fp,"the only significant mode of use of the product.\n\n");

	fprintf(fp,"  \"Installation Information\" for a User Product means any methods,\n");
	fprintf(fp,"procedures, authorization keys, or other information required to install\n");
	fprintf(fp,"and execute modified versions of a covered work in that User Product from\n");
	fprintf(fp,"a modified version of its Corresponding Source.  The information must\n");
	fprintf(fp,"suffice to ensure that the continued functioning of the modified object\n");
	fprintf(fp,"code is in no case prevented or interfered with solely because\n");
	fprintf(fp,"modification has been made.\n\n");

	fprintf(fp,"  If you convey an object code work under this section in, or with, or\n");
	fprintf(fp,"specifically for use in, a User Product, and the conveying occurs as\n");
	fprintf(fp,"part of a transaction in which the right of possession and use of the\n");
	fprintf(fp,"User Product is transferred to the recipient in perpetuity or for a\n");
	fprintf(fp,"fixed term (regardless of how the transaction is characterized), the\n");
	fprintf(fp,"Corresponding Source conveyed under this section must be accompanied\n");
	fprintf(fp,"by the Installation Information.  But this requirement does not apply\n");
	fprintf(fp,"if neither you nor any third party retains the ability to install\n");
	fprintf(fp,"modified object code on the User Product (for example, the work has\n");
	fprintf(fp,"been installed in ROM).\n\n");

	fprintf(fp,"  The requirement to provide Installation Information does not include a\n");
	fprintf(fp,"requirement to continue to provide support service, warranty, or updates\n");
	fprintf(fp,"for a work that has been modified or installed by the recipient, or for\n");
	fprintf(fp,"the User Product in which it has been modified or installed.  Access to a\n");
	fprintf(fp,"network may be denied when the modification itself materially and\n");
	fprintf(fp,"adversely affects the operation of the network or violates the rules and\n");
	fprintf(fp,"protocols for communication across the network.\n\n");

	fprintf(fp,"  Corresponding Source conveyed, and Installation Information provided,\n");
	fprintf(fp,"in accord with this section must be in a format that is publicly\n");
	fprintf(fp,"documented (and with an implementation available to the public in\n");
	fprintf(fp,"source code form), and must require no special password or key for\n");
	fprintf(fp,"unpacking, reading or copying.\n\n");

	fprintf(fp,"  7. Additional Terms.\n\n");

	fprintf(fp,"  \"Additional permissions\" are terms that supplement the terms of this\n");
	fprintf(fp,"License by making exceptions from one or more of its conditions.\n");
	fprintf(fp,"Additional permissions that are applicable to the entire Program shall\n");
	fprintf(fp,"be treated as though they were included in this License, to the extent\n");
	fprintf(fp,"that they are valid under applicable law.  If additional permissions\n");
	fprintf(fp,"apply only to part of the Program, that part may be used separately\n");
	fprintf(fp,"under those permissions, but the entire Program remains governed by\n");
	fprintf(fp,"this License without regard to the additional permissions.\n\n");

	fprintf(fp,"  When you convey a copy of a covered work, you may at your option\n");
	fprintf(fp,"remove any additional permissions from that copy, or from any part of\n");
	fprintf(fp,"it.  (Additional permissions may be written to require their own\n");
	fprintf(fp,"removal in certain cases when you modify the work.)  You may place\n");
	fprintf(fp,"additional permissions on material, added by you to a covered work,\n");
	fprintf(fp,"for which you have or can give appropriate copyright permission.\n\n");

	fprintf(fp,"  Notwithstanding any other provision of this License, for material you\n");
	fprintf(fp,"add to a covered work, you may (if authorized by the copyright holders of\n");
	fprintf(fp,"that material) supplement the terms of this License with terms:\n\n");

	fprintf(fp,"    a) Disclaiming warranty or limiting liability differently from the\n");
	fprintf(fp,"    terms of sections 15 and 16 of this License; or\n\n");

	fprintf(fp,"    b) Requiring preservation of specified reasonable legal notices or\n");
	fprintf(fp,"    author attributions in that material or in the Appropriate Legal\n");
	fprintf(fp,"    Notices displayed by works containing it; or\n\n");

	fprintf(fp,"    c) Prohibiting misrepresentation of the origin of that material, or\n");
	fprintf(fp,"    requiring that modified versions of such material be marked in\n");
	fprintf(fp,"    reasonable ways as different from the original version; or\n\n");

	fprintf(fp,"    d) Limiting the use for publicity purposes of names of licensors or\n");
	fprintf(fp,"    authors of the material; or\n\n");

	fprintf(fp,"    e) Declining to grant rights under trademark law for use of some\n");
	fprintf(fp,"    trade names, trademarks, or service marks; or\n\n");

	fprintf(fp,"    f) Requiring indemnification of licensors and authors of that\n");
	fprintf(fp,"    material by anyone who conveys the material (or modified versions of\n");
	fprintf(fp,"    it) with contractual assumptions of liability to the recipient, for\n");
	fprintf(fp,"    any liability that these contractual assumptions directly impose on\n");
	fprintf(fp,"    those licensors and authors.\n\n");

	fprintf(fp,"  All other non-permissive additional terms are considered \"further\n");
	fprintf(fp,"restrictions\" within the meaning of section 10.  If the Program as you\n");
	fprintf(fp,"received it, or any part of it, contains a notice stating that it is\n");
	fprintf(fp,"governed by this License along with a term that is a further\n");
	fprintf(fp,"restriction, you may remove that term.  If a license document contains\n");
	fprintf(fp,"a further restriction but permits relicensing or conveying under this\n");
	fprintf(fp,"License, you may add to a covered work material governed by the terms\n");
	fprintf(fp,"of that license document, provided that the further restriction does\n");
	fprintf(fp,"not survive such relicensing or conveying.\n\n");

	fprintf(fp,"  If you add terms to a covered work in accord with this section, you\n");
	fprintf(fp,"must place, in the relevant source files, a statement of the\n");
	fprintf(fp,"additional terms that apply to those files, or a notice indicating\n");
	fprintf(fp,"where to find the applicable terms.\n\n");

	fprintf(fp,"  Additional terms, permissive or non-permissive, may be stated in the\n");
	fprintf(fp,"form of a separately written license, or stated as exceptions;\n");
	fprintf(fp,"the above requirements apply either way.\n\n");

	fprintf(fp,"  8. Termination.\n\n");

	fprintf(fp,"  You may not propagate or modify a covered work except as expressly\n");
	fprintf(fp,"provided under this License.  Any attempt otherwise to propagate or\n");
	fprintf(fp,"modify it is void, and will automatically terminate your rights under\n");
	fprintf(fp,"this License (including any patent licenses granted under the third\n");
	fprintf(fp,"paragraph of section 11).\n\n");

	fprintf(fp,"  However, if you cease all violation of this License, then your\n");
	fprintf(fp,"license from a particular copyright holder is reinstated (a)\n");
	fprintf(fp,"provisionally, unless and until the copyright holder explicitly and\n");
	fprintf(fp,"finally terminates your license, and (b) permanently, if the copyright\n");
	fprintf(fp,"holder fails to notify you of the violation by some reasonable means\n");
	fprintf(fp,"prior to 60 days after the cessation.\n\n");

	fprintf(fp,"  Moreover, your license from a particular copyright holder is\n");
	fprintf(fp,"reinstated permanently if the copyright holder notifies you of the\n");
	fprintf(fp,"violation by some reasonable means, this is the first time you have\n");
	fprintf(fp,"received notice of violation of this License (for any work) from that\n");
	fprintf(fp,"copyright holder, and you cure the violation prior to 30 days after\n");
	fprintf(fp,"your receipt of the notice.\n\n");

	fprintf(fp,"  Termination of your rights under this section does not terminate the\n");
	fprintf(fp,"licenses of parties who have received copies or rights from you under\n");
	fprintf(fp,"this License.  If your rights have been terminated and not permanently\n");
	fprintf(fp,"reinstated, you do not qualify to receive new licenses for the same\n");
	fprintf(fp,"material under section 10.\n\n");

	fprintf(fp,"  9. Acceptance Not Required for Having Copies.\n\n");

	fprintf(fp,"  You are not required to accept this License in order to receive or\n");
	fprintf(fp,"run a copy of the Program.  Ancillary propagation of a covered work\n");
	fprintf(fp,"occurring solely as a consequence of using peer-to-peer transmission\n");
	fprintf(fp,"to receive a copy likewise does not require acceptance.  However,\n");
	fprintf(fp,"nothing other than this License grants you permission to propagate or\n");
	fprintf(fp,"modify any covered work.  These actions infringe copyright if you do\n");
	fprintf(fp,"not accept this License.  Therefore, by modifying or propagating a\n");
	fprintf(fp,"covered work, you indicate your acceptance of this License to do so.\n\n");

	fprintf(fp,"  10. Automatic Licensing of Downstream Recipients.\n\n");

	fprintf(fp,"  Each time you convey a covered work, the recipient automatically\n");
	fprintf(fp,"receives a license from the original licensors, to run, modify and\n");
	fprintf(fp,"propagate that work, subject to this License.  You are not responsible\n");
	fprintf(fp,"for enforcing compliance by third parties with this License.\n\n");

	fprintf(fp,"  An \"entity transaction\" is a transaction transferring control of an\n");
	fprintf(fp,"organization, or substantially all assets of one, or subdividing an\n");
	fprintf(fp,"organization, or merging organizations.  If propagation of a covered\n");
	fprintf(fp,"work results from an entity transaction, each party to that\n");
	fprintf(fp,"transaction who receives a copy of the work also receives whatever\n");
	fprintf(fp,"licenses to the work the party's predecessor in interest had or could\n");
	fprintf(fp,"give under the previous paragraph, plus a right to possession of the\n");
	fprintf(fp,"Corresponding Source of the work from the predecessor in interest, if\n");
	fprintf(fp,"the predecessor has it or can get it with reasonable efforts.\n\n");

	fprintf(fp,"  You may not impose any further restrictions on the exercise of the\n");
	fprintf(fp,"rights granted or affirmed under this License.  For example, you may\n");
	fprintf(fp,"not impose a license fee, royalty, or other charge for exercise of\n");
	fprintf(fp,"rights granted under this License, and you may not initiate litigation\n");
	fprintf(fp,"(including a cross-claim or counterclaim in a lawsuit) alleging that\n");
	fprintf(fp,"any patent claim is infringed by making, using, selling, offering for\n");
	fprintf(fp,"sale, or importing the Program or any portion of it.\n\n");

	fprintf(fp,"  11. Patents.\n\n");

	fprintf(fp,"  A \"contributor\" is a copyright holder who authorizes use under this\n");
	fprintf(fp,"License of the Program or a work on which the Program is based.  The\n");
	fprintf(fp,"work thus licensed is called the contributor's \"contributor version\".\n");

	fprintf(fp,"  A contributor's \"essential patent claims\" are all patent claims\n");
	fprintf(fp,"owned or controlled by the contributor, whether already acquired or\n");
	fprintf(fp,"hereafter acquired, that would be infringed by some manner, permitted\n");
	fprintf(fp,"by this License, of making, using, or selling its contributor version,\n");
	fprintf(fp,"but do not include claims that would be infringed only as a\n");
	fprintf(fp,"consequence of further modification of the contributor version.  For\n");
	fprintf(fp,"purposes of this definition, \"control\" includes the right to grant\n");
	fprintf(fp,"patent sublicenses in a manner consistent with the requirements of\n");
	fprintf(fp,"this License.\n\n");

	fprintf(fp,"  Each contributor grants you a non-exclusive, worldwide, royalty-free\n");
	fprintf(fp,"patent license under the contributor's essential patent claims, to\n");
	fprintf(fp,"make, use, sell, offer for sale, import and otherwise run, modify and\n");
	fprintf(fp,"propagate the contents of its contributor version.\n\n");

	fprintf(fp,"  In the following three paragraphs, a \"patent license\" is any express\n");
	fprintf(fp,"agreement or commitment, however denominated, not to enforce a patent\n");
	fprintf(fp,"(such as an express permission to practice a patent or covenant not to\n");
	fprintf(fp,"sue for patent infringement).  To \"grant\" such a patent license to a\n");
	fprintf(fp,"party means to make such an agreement or commitment not to enforce a\n");
	fprintf(fp,"patent against the party.\n\n");

	fprintf(fp,"  If you convey a covered work, knowingly relying on a patent license,\n");
	fprintf(fp,"and the Corresponding Source of the work is not available for anyone\n");
	fprintf(fp,"to copy, free of charge and under the terms of this License, through a\n");
	fprintf(fp,"publicly available network server or other readily accessible means,\n");
	fprintf(fp,"then you must either (1) cause the Corresponding Source to be so\n");
	fprintf(fp,"available, or (2) arrange to deprive yourself of the benefit of the\n");
	fprintf(fp,"patent license for this particular work, or (3) arrange, in a manner\n");
	fprintf(fp,"consistent with the requirements of this License, to extend the patent\n");
	fprintf(fp,"license to downstream recipients.  \"Knowingly relying\" means you have\n");
	fprintf(fp,"actual knowledge that, but for the patent license, your conveying the\n");
	fprintf(fp,"covered work in a country, or your recipient's use of the covered work\n");
	fprintf(fp,"in a country, would infringe one or more identifiable patents in that\n");
	fprintf(fp,"country that you have reason to believe are valid.\n\n");

	fprintf(fp,"  If, pursuant to or in connection with a single transaction or\n");
	fprintf(fp,"arrangement, you convey, or propagate by procuring conveyance of, a\n");
	fprintf(fp,"covered work, and grant a patent license to some of the parties\n");
	fprintf(fp,"receiving the covered work authorizing them to use, propagate, modify\n");
	fprintf(fp,"or convey a specific copy of the covered work, then the patent license\n");
	fprintf(fp,"you grant is automatically extended to all recipients of the covered\n");
	fprintf(fp,"work and works based on it.\n\n");

	fprintf(fp,"  A patent license is \"discriminatory\" if it does not include within\n");
	fprintf(fp,"the scope of its coverage, prohibits the exercise of, or is\n");
	fprintf(fp,"conditioned on the non-exercise of one or more of the rights that are\n");
	fprintf(fp,"specifically granted under this License.  You may not convey a covered\n");
	fprintf(fp,"work if you are a party to an arrangement with a third party that is\n");
	fprintf(fp,"in the business of distributing software, under which you make payment\n");
	fprintf(fp,"to the third party based on the extent of your activity of conveying\n");
	fprintf(fp,"the work, and under which the third party grants, to any of the\n");
	fprintf(fp,"parties who would receive the covered work from you, a discriminatory\n");
	fprintf(fp,"patent license (a) in connection with copies of the covered work\n");
	fprintf(fp,"conveyed by you (or copies made from those copies), or (b) primarily\n");
	fprintf(fp,"for and in connection with specific products or compilations that\n");
	fprintf(fp,"contain the covered work, unless you entered into that arrangement,\n");
	fprintf(fp,"or that patent license was granted, prior to 28 March 2007.\n\n");

	fprintf(fp,"  Nothing in this License shall be construed as excluding or limiting\n");
	fprintf(fp,"any implied license or other defenses to infringement that may\n");
	fprintf(fp,"otherwise be available to you under applicable patent law.\n\n");

	fprintf(fp,"  12. No Surrender of Others' Freedom.\n\n");

	fprintf(fp,"  If conditions are imposed on you (whether by court order, agreement or\n");
	fprintf(fp,"otherwise) that contradict the conditions of this License, they do not\n");
	fprintf(fp,"excuse you from the conditions of this License.  If you cannot convey a\n");
	fprintf(fp,"covered work so as to satisfy simultaneously your obligations under this\n");
	fprintf(fp,"License and any other pertinent obligations, then as a consequence you may\n");
	fprintf(fp,"not convey it at all.  For example, if you agree to terms that obligate you\n");
	fprintf(fp,"to collect a royalty for further conveying from those to whom you convey\n");
	fprintf(fp,"the Program, the only way you could satisfy both those terms and this\n");
	fprintf(fp,"License would be to refrain entirely from conveying the Program.\n\n");

	fprintf(fp,"  13. Use with the GNU Affero General Public License.\n\n");

	fprintf(fp,"  Notwithstanding any other provision of this License, you have\n");
	fprintf(fp,"permission to link or combine any covered work with a work licensed\n");
	fprintf(fp,"under version 3 of the GNU Affero General Public License into a single\n");
	fprintf(fp,"combined work, and to convey the resulting work.  The terms of this\n");
	fprintf(fp,"License will continue to apply to the part which is the covered work,\n");
	fprintf(fp,"but the special requirements of the GNU Affero General Public License,\n");
	fprintf(fp,"section 13, concerning interaction through a network will apply to the\n");
	fprintf(fp,"combination as such.\n\n");

	fprintf(fp,"  14. Revised Versions of this License.\n\n");

	fprintf(fp,"  The Free Software Foundation may publish revised and/or new versions of\n");
	fprintf(fp,"the GNU General Public License from time to time.  Such new versions will\n");
	fprintf(fp,"be similar in spirit to the present version, but may differ in detail to\n");
	fprintf(fp,"address new problems or concerns.\n\n");

	fprintf(fp,"  Each version is given a distinguishing version number.  If the\n");
	fprintf(fp,"Program specifies that a certain numbered version of the GNU General\n");
	fprintf(fp,"Public License \"or any later version\" applies to it, you have the\n");
	fprintf(fp,"option of following the terms and conditions either of that numbered\n");
	fprintf(fp,"version or of any later version published by the Free Software\n");
	fprintf(fp,"Foundation.  If the Program does not specify a version number of the\n");
	fprintf(fp,"GNU General Public License, you may choose any version ever published\n");
	fprintf(fp,"by the Free Software Foundation.\n\n");

	fprintf(fp,"  If the Program specifies that a proxy can decide which future\n");
	fprintf(fp,"versions of the GNU General Public License can be used, that proxy's\n");
	fprintf(fp,"public statement of acceptance of a version permanently authorizes you\n");
	fprintf(fp,"to choose that version for the Program.\n\n");

	fprintf(fp,"  Later license versions may give you additional or different\n");
	fprintf(fp,"permissions.  However, no additional obligations are imposed on any\n");
	fprintf(fp,"author or copyright holder as a result of your choosing to follow a\n");
	fprintf(fp,"later version.\n\n");

	fprintf(fp,"  15. Disclaimer of Warranty.\n\n");

	fprintf(fp,"  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n");
	fprintf(fp,"APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n");
	fprintf(fp,"HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n");
	fprintf(fp,"OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n");
	fprintf(fp,"THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n");
	fprintf(fp,"PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n");
	fprintf(fp,"IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n");
	fprintf(fp,"ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n\n");

	fprintf(fp,"  16. Limitation of Liability.\n\n");

	fprintf(fp,"  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n");
	fprintf(fp,"WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS\n");
	fprintf(fp,"THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY\n");
	fprintf(fp,"GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE\n");
	fprintf(fp,"USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF\n");
	fprintf(fp,"DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD\n");
	fprintf(fp,"PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),\n");
	fprintf(fp,"EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF\n");
	fprintf(fp,"SUCH DAMAGES.\n\n");

	fprintf(fp,"  17. Interpretation of Sections 15 and 16.\n\n");

	fprintf(fp,"  If the disclaimer of warranty and limitation of liability provided\n");
	fprintf(fp,"above cannot be given local legal effect according to their terms,\n");
	fprintf(fp,"reviewing courts shall apply local law that most closely approximates\n");
	fprintf(fp,"an absolute waiver of all civil liability in connection with the\n");
	fprintf(fp,"Program, unless a warranty or assumption of liability accompanies a\n");
	fprintf(fp,"copy of the Program in return for a fee.\n");

	fclose(fp);
}

/* saves a LICENSE file for MIT */
static void save_license_mit(char * filename)
{
	FILE * fp;
	time_t rawtime;
	struct tm * timeinfo;
	int year;
	char email_address[BLOCK_SIZE];

	get_setting("email",email_address);

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"The MIT License (MIT)\n\n");

	fprintf(fp,"Copyright (c) %d %s\n\n",year,email_address);

	fprintf(fp,"Permission is hereby granted, free of charge, to any person obtaining a copy\n");
	fprintf(fp,"of this software and associated documentation files (the \"Software\"), to deal\n");
	fprintf(fp,"in the Software without restriction, including without limitation the rights\n");
	fprintf(fp,"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
	fprintf(fp,"copies of the Software, and to permit persons to whom the Software is\n");
	fprintf(fp,"furnished to do so, subject to the following conditions:\n\n");

	fprintf(fp,"The above copyright notice and this permission notice shall be included in\n");
	fprintf(fp,"all copies or substantial portions of the Software.\n\n");

	fprintf(fp,"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
	fprintf(fp,"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
	fprintf(fp,"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
	fprintf(fp,"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
	fprintf(fp,"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
	fprintf(fp,"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN\n");
	fprintf(fp,"THE SOFTWARE.\n");

	fclose(fp);
}

/* saves a LICENSE file for BSD */
static void save_license_bsd(char * filename)
{
	FILE * fp;
	time_t rawtime;
	struct tm * timeinfo;
	int year;
	char email_address[BLOCK_SIZE];

	get_setting("email",email_address);

	/* get the current year */
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	year = timeinfo->tm_year + 1900;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"Copyright (c) %d, %s\n",year,email_address);
	fprintf(fp,"All rights reserved.\n\n");

	fprintf(fp,"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n\n");

	fprintf(fp,"    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n");
	fprintf(fp,"    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n");
	fprintf(fp,"    Neither the name of the <ORGANIZATION> nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n\n");

	fprintf(fp,"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");

	fclose(fp);
}

/* saves a LICENSE file for Apache */
static void save_license_apache(char * filename)
{
	FILE * fp;

	fp = fopen(filename,"w");
	if (!fp) return;

	fprintf(fp,"Apache License\n\n");

	fprintf(fp,"Version 2.0, January 2004\n\n");

	fprintf(fp,"http://www.apache.org/licenses/\n\n");

	fprintf(fp,"TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n\n");

	fprintf(fp,"1. Definitions.\n\n");

	fprintf(fp,"\"License\" shall mean the terms and conditions for use, reproduction, and distribution as defined by Sections 1 through 9 of this document.\n\n");

	fprintf(fp,"\"Licensor\" shall mean the copyright owner or entity authorized by the copyright owner that is granting the License.\n\n");

	fprintf(fp,"\"Legal Entity\" shall mean the union of the acting entity and all other entities that control, are controlled by, or are under common control with that entity. For the purposes of this definition, \"control\" means (i) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (ii) ownership of fifty percent (50%%) or more of the outstanding shares, or (iii) beneficial ownership of such entity.\n\n");

	fprintf(fp,"\"You\" (or \"Your\") shall mean an individual or Legal Entity exercising permissions granted by this License.\n\n");

	fprintf(fp,"\"Source\" form shall mean the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files.\n\n");

	fprintf(fp,"\"Object\" form shall mean any form resulting from mechanical transformation or translation of a Source form, including but not limited to compiled object code, generated documentation, and conversions to other media types.\n\n");

	fprintf(fp,"\"Work\" shall mean the work of authorship, whether in Source or Object form, made available under the License, as indicated by a copyright notice that is included in or attached to the work (an example is provided in the Appendix below).\n\n");

	fprintf(fp,"\"Derivative Works\" shall mean any work, whether in Source or Object form, that is based on (or derived from) the Work and for which the editorial revisions, annotations, elaborations, or other modifications represent, as a whole, an original work of authorship. For the purposes of this License, Derivative Works shall not include works that remain separable from, or merely link (or bind by name) to the interfaces of, the Work and Derivative Works thereof.\n\n");

	fprintf(fp,"\"Contribution\" shall mean any work of authorship, including the original version of the Work and any modifications or additions to that Work or Derivative Works thereof, that is intentionally submitted to Licensor for inclusion in the Work by the copyright owner or by an individual or Legal Entity authorized to submit on behalf of the copyright owner. For the purposes of this definition, \"submitted\" means any form of electronic, verbal, or written communication sent to the Licensor or its representatives, including but not limited to communication on electronic mailing lists, source code control systems, and issue tracking systems that are managed by, or on behalf of, the Licensor for the purpose of discussing and improving the Work, but excluding communication that is conspicuously marked or otherwise designated in writing by the copyright owner as \"Not a Contribution.\"\n\n");

	fprintf(fp,"\"Contributor\" shall mean Licensor and any individual or Legal Entity on behalf of whom a Contribution has been received by Licensor and subsequently incorporated within the Work.\n\n");

	fprintf(fp,"2. Grant of Copyright License. Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable copyright license to reproduce, prepare Derivative Works of, publicly display, publicly perform, sublicense, and distribute the Work and such Derivative Works in Source or Object form.\n\n");

	fprintf(fp,"3. Grant of Patent License. Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable (except as stated in this section) patent license to make, have made, use, offer to sell, sell, import, and otherwise transfer the Work, where such license applies only to those patent claims licensable by such Contributor that are necessarily infringed by their Contribution(s) alone or by combination of their Contribution(s) with the Work to which such Contribution(s) was submitted. If You institute patent litigation against any entity (including a cross-claim or counterclaim in a lawsuit) alleging that the Work or a Contribution incorporated within the Work constitutes direct or contributory patent infringement, then any patent licenses granted to You under this License for that Work shall terminate as of the date such litigation is filed.\n\n");

	fprintf(fp,"4. Redistribution. You may reproduce and distribute copies of the Work or Derivative Works thereof in any medium, with or without modifications, and in Source or Object form, provided that You meet the following conditions:\n\n");

	fprintf(fp,"    You must give any other recipients of the Work or Derivative Works a copy of this License; and\n");
	fprintf(fp,"    You must cause any modified files to carry prominent notices stating that You changed the files; and\n");
	fprintf(fp,"    You must retain, in the Source form of any Derivative Works that You distribute, all copyright, patent, trademark, and attribution notices from the Source form of the Work, excluding those notices that do not pertain to any part of the Derivative Works; and\n");
	fprintf(fp,"    If the Work includes a \"NOTICE\" text file as part of its distribution, then any Derivative Works that You distribute must include a readable copy of the attribution notices contained within such NOTICE file, excluding those notices that do not pertain to any part of the Derivative Works, in at least one of the following places: within a NOTICE text file distributed as part of the Derivative Works; within the Source form or documentation, if provided along with the Derivative Works; or, within a display generated by the Derivative Works, if and wherever such third-party notices normally appear. The contents of the NOTICE file are for informational purposes only and do not modify the License. You may add Your own attribution notices within Derivative Works that You distribute, alongside or as an addendum to the NOTICE text from the Work, provided that such additional attribution notices cannot be construed as modifying the License.\n\n");

	fprintf(fp,"    You may add Your own copyright statement to Your modifications and may provide additional or different license terms and conditions for use, reproduction, or distribution of Your modifications, or for any such Derivative Works as a whole, provided Your use, reproduction, and distribution of the Work otherwise complies with the conditions stated in this License.\n\n");

	fprintf(fp,"5. Submission of Contributions. Unless You explicitly state otherwise, any Contribution intentionally submitted for inclusion in the Work by You to the Licensor shall be under the terms and conditions of this License, without any additional terms or conditions. Notwithstanding the above, nothing herein shall supersede or modify the terms of any separate license agreement you may have executed with Licensor regarding such Contributions.\n\n");

	fprintf(fp,"6. Trademarks. This License does not grant permission to use the trade names, trademarks, service marks, or product names of the Licensor, except as required for reasonable and customary use in describing the origin of the Work and reproducing the content of the NOTICE file.\n\n");

	fprintf(fp,"7. Disclaimer of Warranty. Unless required by applicable law or agreed to in writing, Licensor provides the Work (and each Contributor provides its Contributions) on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied, including, without limitation, any warranties or conditions of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE. You are solely responsible for determining the appropriateness of using or redistributing the Work and assume any risks associated with Your exercise of permissions under this License.\n\n");

	fprintf(fp,"8. Limitation of Liability. In no event and under no legal theory, whether in tort (including negligence), contract, or otherwise, unless required by applicable law (such as deliberate and grossly negligent acts) or agreed to in writing, shall any Contributor be liable to You for damages, including any direct, indirect, special, incidental, or consequential damages of any character arising as a result of this License or out of the use or inability to use the Work (including but not limited to damages for loss of goodwill, work stoppage, computer failure or malfunction, or any and all other commercial damages or losses), even if such Contributor has been advised of the possibility of such damages.\n\n");

	fprintf(fp,"9. Accepting Warranty or Additional Liability. While redistributing the Work or Derivative Works thereof, You may choose to offer, and charge a fee for, acceptance of support, warranty, indemnity, or other liability obligations and/or rights consistent with this License. However, in accepting such obligations, You may act only on Your own behalf and on Your sole responsibility, not on behalf of any other Contributor, and only if You agree to indemnify, defend, and hold each Contributor harmless for any liability incurred by, or claims asserted against, such Contributor by reason of your accepting any such warranty or additional liability.\n");

	fclose(fp);
}

/* returns a non-zero value if the given license is valid */
int valid_license(char * license)
{
	int i;

	/* convert to lower case */
	for (i = 0; i < strlen(license); i++) {
		license[i] = tolower(license[i]);
	}

	/* check against a list of known licenses */
	if ((strcmp(license,"gpl2")==0) ||
		(strcmp(license,"gpl3")==0) ||
		(strcmp(license,"mit")==0) ||
		(strcmp(license,"bsd")==0) ||
		(strcmp(license,"apache")==0) ||
		(strcmp(license,"mozilla")==0) ||
		(strcmp(license,"mpl")==0)) {
		return 1;
	}
	return 0;
}

/* saves a file called LICENSE */
int save_license(char * directory)
{
	int i;
	char license[BLOCK_SIZE];
	char filename[BLOCK_SIZE];

	sprintf(filename,"%s%cLICENSE",directory,DIRECTORY_SEPARATOR);

	/* does the license file already exist? */
	if (file_exists(filename)!=0) return -1;

	/* get the type of license */
	get_setting("license",license);

	/* convert to lower case */
	for (i = 0; i < strlen(license); i++) {
		license[i] = tolower(license[i]);
	}

	if (strcmp(license,"gpl2")==0) {
		save_license_gpl2(filename);
		return 0;
	}
	if (strcmp(license,"gpl3")==0) {
		save_license_gpl3(filename);
		return 0;
	}
	if (strcmp(license,"mit")==0) {
		save_license_mit(filename);
		return 0;
	}
	if (strcmp(license,"bsd")==0) {
		save_license_bsd(filename);
		return 0;
	}
	if (strcmp(license,"apache")==0) {
		save_license_apache(filename);
		return 0;
	}
	if ((strcmp(license,"mozilla")==0) ||
		(strcmp(license,"mpl")==0)) {
		save_license_mozilla(filename);
		return 0;
	}
	return -1;
}
